#pragma once

#include "halls/HallBase.hpp"
#include <memory>

class FloorRenderable;

class FamilyHall : public HallBase {
public:
    FamilyHall();
    ~FamilyHall();

    void Init() override;
    void Update(float dt) override;
    void RenderOpaque() override;
    void RenderTransparent() override;
    void Destroy() override;

private:
    std::unique_ptr<FloorRenderable> m_floor;
};
