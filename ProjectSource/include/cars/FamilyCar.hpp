#pragma once

#include "cars/CarBase.hpp"

class FamilyCar : public CarBase {
public:
    FamilyCar();
    ~FamilyCar();

    void Init() override;
    void Update(float dt) override;
    void RenderOpaque() override;
    void RenderTransparent() override;
    void Destroy() override;
};
