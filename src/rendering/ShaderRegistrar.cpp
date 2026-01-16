#include "rendering/ShaderRegistrar.hpp"

std::unordered_map<std::string, int> ShaderRegistrar::s_shaders;

void ShaderRegistrar::Init() {
    // stub: register a default shader id
    s_shaders["default"] = 1;
}

int ShaderRegistrar::Get(const std::string& name) {
    auto it = s_shaders.find(name);
    return it != s_shaders.end() ? it->second : 0;
}

void ShaderRegistrar::Register(const std::string& name, int id) {
    s_shaders[name] = id;
}

void ShaderRegistrar::Shutdown() {
    s_shaders.clear();
}
