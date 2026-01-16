#pragma once

#include <string>
#include <unordered_map>

class Shader {
public:
    Shader() = default;
    ~Shader();

    // Compile from GLSL source strings
    bool CompileFromSource(const std::string& vertexSrc, const std::string& fragmentSrc);

    // Compile from file paths (relative to working dir/runtime)
    bool CompileFromFiles(const std::string& vertexPath, const std::string& fragmentPath);

    void Use() const;
    int GetID() const { return static_cast<int>(m_program); }

    // Uniform helpers
    void SetUniformMat4(const std::string& name, const float* matrix);
    void SetUniformInt(const std::string& name, int value);
    void SetUniformFloat(const std::string& name, float value);

private:
    unsigned int m_program = 0;
    std::unordered_map<std::string, int> m_locationCache;

    int GetUniformLocation(const std::string& name);
    bool CheckCompileErrors(unsigned int object, const std::string& type);
};