#include "cars/MainCar.hpp"

MainCar::MainCar() { m_name = "main"; }
MainCar::~MainCar() {}

void MainCar::Init() {}
void MainCar::Update(float dt) { (void)dt; }
void MainCar::RenderOpaque() {}
void MainCar::RenderTransparent() {}
void MainCar::Destroy() {}
