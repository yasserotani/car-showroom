#pragma once

#include <string>
#include <unordered_map>
#include <memory>

class CarBase;

class CarRegistrar {
public:
    static void Init();
    static CarBase* Get(const std::string& name);
    static void Register(const std::string& name, std::unique_ptr<CarBase> car);
    static void Shutdown();

private:
    static std::unordered_map<std::string, std::unique_ptr<CarBase>> s_cars;
};
