#pragma once

#include "cars/CarBase.hpp"

class SimpleCarSport : public CarBase {
public:
    SimpleCarSport();
    ~SimpleCarSport();

    void Init() override;
    void Update(float dt) override;
    void RenderOpaque() override;
    void RenderTransparent() override;
    void Destroy() override;
};
