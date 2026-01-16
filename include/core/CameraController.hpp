#pragma once

#include "core/Camera.hpp"

class CameraController {
public:
    CameraController() = default;
    ~CameraController() = default;

    void Init();
    void Update(float dt);
    void SetViewport(int w, int h);

    // Access the internal camera used by renderables
    Camera& GetCamera() { return m_camera; }

private:
    // camera state
    Camera m_camera;
    float m_yaw = -90.0f;   // degrees
    float m_pitch = -20.0f; // degrees, slight look-down
    float m_speed = 10.0f;  // units / second
    float m_sensitivity = 0.15f; // mouse sensitivity

    // viewport
    int m_width = 1280;
    int m_height = 720;

    // helpers
    void UpdateViewMatrix();
    void UpdateProjectionMatrix();
};
