#pragma once

#include "halls/HallBase.hpp"
#include <memory>

class FloorRenderable;

class LuxuryHall : public HallBase {
public:
    LuxuryHall();
    ~LuxuryHall();

    void Init() override;
    void Update(float dt) override;
    void RenderOpaque() override;
    void RenderTransparent() override;
    void Destroy() override;

private:
    std::unique_ptr<FloorRenderable> m_floor;
};
