#include "registrars/CarRegistrar.hpp"
#include "cars/SimpleCarEV.hpp"
#include "cars/SimpleCarLuxury.hpp"
#include "cars/SimpleCarSport.hpp"
#include "cars/FamilyCar.hpp"
#include "cars/MainCar.hpp"

#include <iostream>

std::unordered_map<std::string, std::unique_ptr<CarBase>> CarRegistrar::s_cars;

void CarRegistrar::Init() {
    // Register a set of default cars
    Register("simple_ev", std::make_unique<SimpleCarEV>());
    Register("simple_luxury", std::make_unique<SimpleCarLuxury>());
    Register("simple_sport", std::make_unique<SimpleCarSport>());
    Register("family", std::make_unique<FamilyCar>());
    Register("main", std::make_unique<MainCar>());

    // Initialize all cars
    for (auto& kv : s_cars) {
        if (kv.second) kv.second->Init();
    }
}

CarBase* CarRegistrar::Get(const std::string& name) {
    auto it = s_cars.find(name);
    return it != s_cars.end() ? it->second.get() : nullptr;
}

void CarRegistrar::Register(const std::string& name, std::unique_ptr<CarBase> car) {
    s_cars[name] = std::move(car);
}

void CarRegistrar::Shutdown() {
    s_cars.clear();
}
