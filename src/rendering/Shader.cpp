#include "rendering/Shader.hpp"
#include "rendering/Renderer.hpp"
#include <glad/glad.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <filesystem>

static std::string ReadFileToString(const std::string& path) {
    std::ifstream file(path);
    if (!file) return std::string();
    std::ostringstream ss;
    ss << file.rdbuf();
    return ss.str();
}

Shader::~Shader() {
    if (m_program) glDeleteProgram(m_program);
}

bool Shader::CompileFromSource(const std::string& vertexSrc, const std::string& fragmentSrc) {
    unsigned int vertex = glCreateShader(GL_VERTEX_SHADER);
    const char* vsrc = vertexSrc.c_str();
    glShaderSource(vertex, 1, &vsrc, nullptr);
    glCompileShader(vertex);
    if (!CheckCompileErrors(vertex, "VERTEX")) {
        glDeleteShader(vertex);
        return false;
    }

    unsigned int fragment = glCreateShader(GL_FRAGMENT_SHADER);
    const char* fsrc = fragmentSrc.c_str();
    glShaderSource(fragment, 1, &fsrc, nullptr);
    glCompileShader(fragment);
    if (!CheckCompileErrors(fragment, "FRAGMENT")) {
        glDeleteShader(vertex);
        glDeleteShader(fragment);
        return false;
    }

    m_program = glCreateProgram();
    glAttachShader(m_program, vertex);
    glAttachShader(m_program, fragment);
    glLinkProgram(m_program);
    if (!CheckCompileErrors(m_program, "PROGRAM")) {
        glDeleteShader(vertex);
        glDeleteShader(fragment);
        glDeleteProgram(m_program);
        m_program = 0;
        return false;
    }

    // Cleanup shaders; program holds the linked code
    glDetachShader(m_program, vertex);
    glDetachShader(m_program, fragment);
    glDeleteShader(vertex);
    glDeleteShader(fragment);

    return true;
}

bool Shader::CompileFromFiles(const std::string& vertexPath, const std::string& fragmentPath) {
    // Try to read both files
    std::string v = ReadFileToString(vertexPath);
    std::string f = ReadFileToString(fragmentPath);

    if (v.empty() || f.empty()) {
        std::cerr << "Shader::CompileFromFiles: failed to read files '" << vertexPath << "' or '" << fragmentPath << "'\n";
        // Search upward from current path for a 'shaders' directory and attempt to load by filename
        std::filesystem::path cur = std::filesystem::current_path();
        bool found = false;
        for (int depth = 0; depth < 6 && !found; ++depth) {
            std::filesystem::path cand = cur / "shaders";
            std::filesystem::path candV = cand / std::filesystem::path(vertexPath).filename();
            std::filesystem::path candF = cand / std::filesystem::path(fragmentPath).filename();
            v = ReadFileToString(candV.string());
            f = ReadFileToString(candF.string());
            if (!v.empty() && !f.empty()) {
                found = true;
                break;
            }
            // move up one directory
            if (!cur.has_parent_path()) break;
            cur = cur.parent_path();
        }
        if (!found) {
            std::cerr << "Shader::CompileFromFiles: could not locate shader files for '" << vertexPath << "'/'" << fragmentPath << "'\n";
            return false;
        }
    }

    return CompileFromSource(v, f);
}

void Shader::Use() const {
    if (m_program) glUseProgram(m_program);
    // When a shader is bound, provide camera matrices if available
    if (m_program && Renderer::HasCamera()) {
        // renderer will set uView/uProj on this shader
        Renderer::ApplyCameraToShader(const_cast<Shader&>(*this));
    }
}

int Shader::GetUniformLocation(const std::string& name) {
    auto it = m_locationCache.find(name);
    if (it != m_locationCache.end()) return it->second;
    int loc = glGetUniformLocation(m_program, name.c_str());
    m_locationCache[name] = loc;
    return loc;
}

void Shader::SetUniformMat4(const std::string& name, const float* matrix) {
    int loc = GetUniformLocation(name);
    if (loc >= 0) glUniformMatrix4fv(loc, 1, GL_FALSE, matrix);
}

void Shader::SetUniformInt(const std::string& name, int value) {
    int loc = GetUniformLocation(name);
    if (loc >= 0) glUniform1i(loc, value);
}

void Shader::SetUniformFloat(const std::string& name, float value) {
    int loc = GetUniformLocation(name);
    if (loc >= 0) glUniform1f(loc, value);
}

bool Shader::CheckCompileErrors(unsigned int object, const std::string& type) {
    int success = 0;
    if (type != "PROGRAM") {
        glGetShaderiv(object, GL_COMPILE_STATUS, &success);
        if (!success) {
            int length = 0;
            glGetShaderiv(object, GL_INFO_LOG_LENGTH, &length);
            std::string info(length, '\0');
            glGetShaderInfoLog(object, length, nullptr, &info[0]);
            std::cerr << "Shader compile error (" << type << "): " << info << std::endl;
            return false;
        }
    } else {
        glGetProgramiv(object, GL_LINK_STATUS, &success);
        if (!success) {
            int length = 0;
            glGetProgramiv(object, GL_INFO_LOG_LENGTH, &length);
            std::string info(length, '\0');
            glGetProgramInfoLog(object, length, nullptr, &info[0]);
            std::cerr << "Program link error: " << info << std::endl;
            return false;
        }
    }
    return true;
}