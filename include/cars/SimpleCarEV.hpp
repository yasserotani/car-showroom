#pragma once

#include "cars/CarBase.hpp"

class SimpleCarEV : public CarBase {
public:
    SimpleCarEV();
    ~SimpleCarEV();

    void Init() override;
    void Update(float dt) override;
    void RenderOpaque() override;
    void RenderTransparent() override;
    void Destroy() override;
};
