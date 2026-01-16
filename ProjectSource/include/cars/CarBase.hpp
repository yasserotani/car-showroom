#pragma once

#include <string>

class CarBase {
public:
    CarBase() = default;
    virtual ~CarBase() = default;

    // Lifecycle
    virtual void Init() = 0;
    virtual void Update(float dt) = 0;
    virtual void RenderOpaque() = 0;
    virtual void RenderTransparent() = 0;
    virtual void Destroy() = 0;

    const std::string& GetName() const { return m_name; }

protected:
    std::string m_name;
};
