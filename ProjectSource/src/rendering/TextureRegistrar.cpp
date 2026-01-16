#include "rendering/TextureRegistrar.hpp"

std::unordered_map<std::string, int> TextureRegistrar::s_textures;

void TextureRegistrar::Init() {
    s_textures["default"] = 1;
}

int TextureRegistrar::Get(const std::string& name) {
    auto it = s_textures.find(name);
    return it != s_textures.end() ? it->second : 0;
}

void TextureRegistrar::Register(const std::string& name, int id) {
    s_textures[name] = id;
}

void TextureRegistrar::Shutdown() { s_textures.clear(); }
