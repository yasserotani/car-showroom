#include "rendering/ModelRegistrar.hpp"

std::unordered_map<std::string, int> ModelRegistrar::s_models;

void ModelRegistrar::Init() { s_models["default"] = 1; }
int ModelRegistrar::Get(const std::string& name) { auto it = s_models.find(name); return it != s_models.end() ? it->second : 0; }
void ModelRegistrar::Register(const std::string& name, int id) { s_models[name] = id; }
void ModelRegistrar::Shutdown() { s_models.clear(); }
