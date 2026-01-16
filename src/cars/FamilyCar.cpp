#include "cars/FamilyCar.hpp"
#include <iostream>

FamilyCar::FamilyCar() { m_name = "family_car"; }
FamilyCar::~FamilyCar() {}

void FamilyCar::Init() {
    // simple init stub
}

void FamilyCar::Update(float dt) {
    (void)dt;
}

void FamilyCar::RenderOpaque() {}
void FamilyCar::RenderTransparent() {}
void FamilyCar::Destroy() {}
