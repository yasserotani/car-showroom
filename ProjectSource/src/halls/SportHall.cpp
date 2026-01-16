#include "halls/SportHall.hpp"
#include "registrars/CarRegistrar.hpp"
#include "cars/CarBase.hpp"
#include "rendering/FloorRenderable.hpp"

SportHall::SportHall() { m_name = "sport"; }
SportHall::~SportHall() {}

void SportHall::Init() {
    // Floor at (-25,0,-25)
    m_floor = std::make_unique<FloorRenderable>(-45.0f, 0.0f, -45.0f, 50.0f, 0.2f, 50.0f);

    CarBase* s = CarRegistrar::Get("simple_sport");
    if (s) m_cars.push_back(s);
}

void SportHall::Update(float dt) {
    for (auto* c : m_cars) if (c) c->Update(dt);
}

void SportHall::RenderOpaque() { if (m_floor) m_floor->RenderOpaque(Camera()); for (auto* c : m_cars) if (c) c->RenderOpaque(); }
void SportHall::RenderTransparent() { for (auto* c : m_cars) if (c) c->RenderTransparent(); }
void SportHall::Destroy() { m_cars.clear(); m_floor.reset(); }
