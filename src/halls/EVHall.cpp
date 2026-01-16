#include "halls/EVHall.hpp"
#include "registrars/CarRegistrar.hpp"
#include "cars/CarBase.hpp"
#include "rendering/FloorRenderable.hpp"

EVHall::EVHall() { m_name = "ev"; }
EVHall::~EVHall() {}

void EVHall::Init() {
    // Floor at (25,0,-25) -> (30,0, -30)
    m_floor = std::make_unique<FloorRenderable>(45.0f, 0.0f, -45.0f, 50.0f, 0.2f, 50.0f);

    CarBase* c = CarRegistrar::Get("simple_ev");
    if (c) m_cars.push_back(c);
}

void EVHall::Update(float dt) { for (auto* c : m_cars) if (c) c->Update(dt); }
void EVHall::RenderOpaque() { if (m_floor) m_floor->RenderOpaque(Camera()); for (auto* c : m_cars) if (c) c->RenderOpaque(); }
void EVHall::RenderTransparent() { for (auto* c : m_cars) if (c) c->RenderTransparent(); }
void EVHall::Destroy() { m_cars.clear(); m_floor.reset(); }
