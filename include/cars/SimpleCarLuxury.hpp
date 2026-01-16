#pragma once

#include "cars/CarBase.hpp"

class SimpleCarLuxury : public CarBase {
public:
    SimpleCarLuxury();
    ~SimpleCarLuxury();

    void Init() override;
    void Update(float dt) override;
    void RenderOpaque() override;
    void RenderTransparent() override;
    void Destroy() override;
};
