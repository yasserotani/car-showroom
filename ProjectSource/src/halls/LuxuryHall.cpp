#include "halls/LuxuryHall.hpp"
#include "registrars/CarRegistrar.hpp"
#include "cars/CarBase.hpp"
#include "rendering/FloorRenderable.hpp"

LuxuryHall::LuxuryHall() { m_name = "luxury"; }
LuxuryHall::~LuxuryHall() {}

void LuxuryHall::Init() {
    // Floor at (25,0,25)
    m_floor = std::make_unique<FloorRenderable>(45.0f, 0.0f, 45.0, 50.0f, 0.2f, 50.0f);

    CarBase* c = CarRegistrar::Get("simple_luxury");
    if (c) m_cars.push_back(c);
}

void LuxuryHall::Update(float dt) { for (auto* c : m_cars) if (c) c->Update(dt); }
void LuxuryHall::RenderOpaque() { if (m_floor) m_floor->RenderOpaque(Camera()); for (auto* c : m_cars) if (c) c->RenderOpaque(); }
void LuxuryHall::RenderTransparent() { for (auto* c : m_cars) if (c) c->RenderTransparent(); }
void LuxuryHall::Destroy() { m_cars.clear(); m_floor.reset(); }
