#pragma once

#include "halls/HallBase.hpp"

class CenterHall : public HallBase {
public:
    CenterHall();
    ~CenterHall();

    void Init() override;
    void Update(float dt) override;
    void RenderOpaque() override;
    void RenderTransparent() override;
    void Destroy() override;

private:
    class FloorRenderable* m_floor = nullptr; // forward-declared type in header not needed
};