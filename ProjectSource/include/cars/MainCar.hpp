#pragma once

#include "cars/CarBase.hpp"

class MainCar : public CarBase {
public:
    MainCar();
    ~MainCar();

    void Init() override;
    void Update(float dt) override;
    void RenderOpaque() override;
    void RenderTransparent() override;
    void Destroy() override;
};
