#pragma once

#include "halls/HallBase.hpp"
#include <memory>

class FloorRenderable;

class SportHall : public HallBase {
public:
    SportHall();
    ~SportHall();

    void Init() override;
    void Update(float dt) override;
    void RenderOpaque() override;
    void RenderTransparent() override;
    void Destroy() override;

private:
    std::unique_ptr<FloorRenderable> m_floor;
};
