#include "halls/CenterHall.hpp"
#include "rendering/FloorRenderable.hpp"

CenterHall::CenterHall() { m_name = "center"; }
CenterHall::~CenterHall() {}

void CenterHall::Init() {
    // Floor at (0,0,0) scale (20,0.2,20)
    m_floor = new FloorRenderable(0.0f, 0.-0.1f, 0.0f, 45.0f, 0.2f, 45.0f);
}

void CenterHall::Update(float /*dt*/) {
}

void CenterHall::RenderOpaque() {
    if (m_floor) m_floor->RenderOpaque(Camera());
}

void CenterHall::RenderTransparent() {
    if (m_floor) m_floor->RenderTransparent(Camera());
}

void CenterHall::Destroy() {
    if (m_floor) { delete m_floor; m_floor = nullptr; }
}