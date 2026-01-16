#pragma once

#include "cars/CarBase.hpp"
#include <memory>
#include <vector>

class Mesh;
class Shader;
class Texture;
class Camera;

// Simple component renderer for car parts
struct CarComponent {
    std::unique_ptr<Mesh> mesh;
    std::unique_ptr<Shader> shader;
    Texture* texture = nullptr;
    float position[3] = {0.0f, 0.0f, 0.0f};
    float rotation[3] = {0.0f, 0.0f, 0.0f};
    float scale[3] = {1.0f, 1.0f, 1.0f};
    bool isTransparent = false;
};

class MainCar : public CarBase {
public:
    MainCar();
    ~MainCar();

    void Init() override;
    void Update(float dt) override;
    void RenderOpaque() override;
    void RenderTransparent() override;
    void Destroy() override;

    // Get driver seat position for camera attachment
    void GetDriverSeatPosition(float& x, float& y, float& z) const;
    void GetDriverSeatRotation(float& yaw) const;

private:
    // Car position in world
    float m_position[3] = {0.0f, 0.0f, 0.0f};
    float m_rotation = 0.0f; // yaw rotation in degrees

    // Driver seat position (relative to car center)
    float m_driverSeatPos[3] = {-0.3f, 0.6f, 0.3f}; // slightly left, at seat height, forward
    float m_driverSeatYaw = 0.0f; // facing forward

    // Car components
    std::vector<CarComponent> m_opaqueComponents;
    std::vector<CarComponent> m_transparentComponents;

    void CreateExterior();
    void CreateInterior();
    void CreateWheels();
    
    // Helper to build model matrix
    void BuildModelMatrix(const CarComponent& comp, float out[16]) const;
    void RenderComponent(const CarComponent& comp, const Camera& camera) const;
};
