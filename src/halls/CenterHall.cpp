#include "halls/CenterHall.hpp"
#include "rendering/FloorRenderable.hpp"
#include "registrars/CarRegistrar.hpp"
#include "cars/CarBase.hpp"

CenterHall::CenterHall() { m_name = "center"; }
CenterHall::~CenterHall() {}

void CenterHall::Init() {
    // Floor at (0,0,0) scale (20,0.2,20)
    m_floor = new FloorRenderable(0.0f, 0.-0.1f, 0.0f, 45.0f, 0.2f, 45.0f);

    // Add MainCar to center hall (the detailed car with interior)
    CarBase* mainCar = CarRegistrar::Get("main");
    if (mainCar) {
        m_cars.push_back(mainCar);
    }
}

void CenterHall::Update(float dt) {
    // Update cars
    for (auto* c : m_cars) if (c) c->Update(dt);
}

void CenterHall::RenderOpaque() {
    if (m_floor) m_floor->RenderOpaque(Camera());
    // Render cars
    for (auto* c : m_cars) if (c) c->RenderOpaque();
}

void CenterHall::RenderTransparent() {
    if (m_floor) m_floor->RenderTransparent(Camera());
    // Render transparent car parts (windows, etc.)
    for (auto* c : m_cars) if (c) c->RenderTransparent();
}

void CenterHall::Destroy() {
    if (m_floor) { delete m_floor; m_floor = nullptr; }
    m_cars.clear();
}