#pragma once

#include <string>
#include <unordered_map>

class ModelRegistrar {
public:
    static void Init();
    static int Get(const std::string& name);
    static void Register(const std::string& name, int id);
    static void Shutdown();
private:
    static std::unordered_map<std::string, int> s_models;
};
