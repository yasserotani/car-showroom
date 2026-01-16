#include "cars/SimpleCarEV.hpp"

SimpleCarEV::SimpleCarEV() { m_name = "simple_ev"; }
SimpleCarEV::~SimpleCarEV() {}

void SimpleCarEV::Init() {}
void SimpleCarEV::Update(float dt) { (void)dt; }
void SimpleCarEV::RenderOpaque() {}
void SimpleCarEV::RenderTransparent() {}
void SimpleCarEV::Destroy() {}
