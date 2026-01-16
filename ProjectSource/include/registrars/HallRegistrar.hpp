#pragma once

#include <string>
#include <unordered_map>
#include <memory>

class HallBase;

class HallRegistrar {
public:
    static void Init();
    static HallBase* Get(const std::string& name);
    static void Register(const std::string& name, std::unique_ptr<HallBase> hall);
    static void Shutdown();

    // Return a vector of raw pointers to all registered halls (non-owning)
    static std::vector<HallBase*> GetAll();

private:
    static std::unordered_map<std::string, std::unique_ptr<HallBase>> s_halls;
};
