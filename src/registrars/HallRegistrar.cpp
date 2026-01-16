#include "registrars/HallRegistrar.hpp"
#include "halls/FamilyHall.hpp"
#include "halls/SportHall.hpp"
#include "halls/EVHall.hpp"
#include "halls/LuxuryHall.hpp"
#include "halls/CenterHall.hpp"

#include <iostream>

std::unordered_map<std::string, std::unique_ptr<HallBase>> HallRegistrar::s_halls;

void HallRegistrar::Init() {
    Register("family", std::make_unique<FamilyHall>());
    Register("sport", std::make_unique<SportHall>());
    Register("ev", std::make_unique<EVHall>());
    Register("luxury", std::make_unique<LuxuryHall>());
    Register("center", std::make_unique<CenterHall>());

    for (auto& kv : s_halls) {
        if (kv.second) kv.second->Init();
    }
}

HallBase* HallRegistrar::Get(const std::string& name) {
    auto it = s_halls.find(name);
    return it != s_halls.end() ? it->second.get() : nullptr;
}

void HallRegistrar::Register(const std::string& name, std::unique_ptr<HallBase> hall) {
    s_halls[name] = std::move(hall);
}

std::vector<HallBase*> HallRegistrar::GetAll() {
    std::vector<HallBase*> out;
    out.reserve(s_halls.size());
    for (auto& kv : s_halls) {
        if (kv.second) out.push_back(kv.second.get());
    }
    return out;
}

void HallRegistrar::Shutdown() {
    s_halls.clear();
}
