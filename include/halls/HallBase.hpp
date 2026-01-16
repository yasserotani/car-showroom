#pragma once

#include <string>
#include <vector>

class CarBase;

class HallBase {
public:
    HallBase() = default;
    virtual ~HallBase() = default;

    virtual void Init() = 0;
    virtual void Update(float dt) = 0;
    virtual void RenderOpaque() = 0;
    virtual void RenderTransparent() = 0;
    virtual void Destroy() = 0;

    const std::string& GetName() const { return m_name; }

protected:
    std::string m_name;
    std::vector<CarBase*> m_cars; // non-owning pointers, registrar owns cars
};
