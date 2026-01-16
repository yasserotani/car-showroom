#include "halls/FamilyHall.hpp"
#include "registrars/CarRegistrar.hpp"
#include "cars/CarBase.hpp"
#include "rendering/FloorRenderable.hpp"

FamilyHall::FamilyHall() { m_name = "family"; }
FamilyHall::~FamilyHall() {}

void FamilyHall::Init() {
    // Floor at (-25,0,25) scale (20,0.2,20)
    m_floor = std::make_unique<FloorRenderable>(-45.0f, 0.0f, 45.0f, 50.0f, 0.2f, 50.0f);

    // Add family-specific cars by querying CarRegistrar
    CarBase* c = CarRegistrar::Get("family");
    if (c) m_cars.push_back(c);
}

void FamilyHall::Update(float dt) {
    for (auto* c : m_cars) if (c) c->Update(dt);
}

void FamilyHall::RenderOpaque() {
    if (m_floor) m_floor->RenderOpaque(Camera());
    for (auto* c : m_cars) if (c) c->RenderOpaque();
}

void FamilyHall::RenderTransparent() {
    for (auto* c : m_cars) if (c) c->RenderTransparent();
}

void FamilyHall::Destroy() {
    m_cars.clear();
    m_floor.reset();
}
