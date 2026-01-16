#include "cars/SimpleCarSport.hpp"

SimpleCarSport::SimpleCarSport() { m_name = "simple_sport"; }
SimpleCarSport::~SimpleCarSport() {}

void SimpleCarSport::Init() {}
void SimpleCarSport::Update(float dt) { (void)dt; }
void SimpleCarSport::RenderOpaque() {}
void SimpleCarSport::RenderTransparent() {}
void SimpleCarSport::Destroy() {}
