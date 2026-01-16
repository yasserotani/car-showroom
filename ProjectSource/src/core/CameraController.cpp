#include "core/CameraController.hpp"
#include "core/InputSystem.hpp"
#include <GLFW/glfw3.h>
#include <cmath>
#include <filesystem>
#include <fstream>

static inline float DegToRad(float d) { return d * 3.14159265f / 180.0f; }

static inline void Normalize(float v[3]) {
    float len = std::sqrt(v[0]*v[0] + v[1]*v[1] + v[2]*v[2]);
    if (len > 0.00001f) { v[0]/=len; v[1]/=len; v[2]/=len; }
}

static inline void Cross(const float a[3], const float b[3], float out[3]) {
    out[0] = a[1]*b[2] - a[2]*b[1];
    out[1] = a[2]*b[0] - a[0]*b[2];
    out[2] = a[0]*b[1] - a[1]*b[0];
}

static inline float Dot(const float a[3], const float b[3]) {
    return a[0]*b[0] + a[1]*b[1] + a[2]*b[2];
}

// build lookAt matrix into 16-element column-major array
static void LookAt(const float eye[3], const float center[3], const float up[3], float out[16]) {
    float f[3] = { center[0]-eye[0], center[1]-eye[1], center[2]-eye[2] };
    Normalize(f);
    float s[3];
    Cross(f, up, s);
    Normalize(s);
    float u[3];
    Cross(s, f, u);

    // column-major
    out[0] = s[0]; out[4] = s[1]; out[8]  = s[2]; out[12] = -Dot(s, eye);
    out[1] = u[0]; out[5] = u[1]; out[9]  = u[2]; out[13] = -Dot(u, eye);
    out[2] = -f[0];out[6] = -f[1];out[10] = -f[2];out[14] = Dot(f, eye);
    out[3] = 0.0f; out[7] = 0.0f; out[11] = 0.0f; out[15] = 1.0f;
}

// perspective projection into column-major array
static void Perspective(float fovDeg, float aspect, float znear, float zfar, float out[16]) {
    float f = 1.0f / std::tan(DegToRad(fovDeg) * 0.5f);
    out[0] = f / aspect; out[4] = 0; out[8] = 0; out[12] = 0;
    out[1] = 0; out[5] = f; out[9] = 0; out[13] = 0;
    out[2] = 0; out[6] = 0; out[10] = (zfar + znear) / (znear - zfar); out[14] = (2.0f * zfar * znear) / (znear - zfar);
    out[3] = 0; out[7] = 0; out[11] = -1; out[15] = 0;
}

void CameraController::Init() {
    // Start at specified position looking at origin
    m_camera.view[0]=1;m_camera.view[1]=0;m_camera.view[2]=0;m_camera.view[3]=0; // init identity
    // position and orientation
    float eye[3] = {0.0f, 10.0f, 25.0f};
    float center[3] = {0.0f, 0.0f, 0.0f};
    float up[3] = {0.0f, 1.0f, 0.0f};

    // compute initial yaw/pitch from direction to center
    float dir[3] = { center[0]-eye[0], center[1]-eye[1], center[2]-eye[2] };
    Normalize(dir);
    m_pitch = std::atan2(dir[1], std::sqrt(dir[0]*dir[0] + dir[2]*dir[2])) * 180.0f / 3.14159265f;
    m_yaw = std::atan2(dir[2], dir[0]) * 180.0f / 3.14159265f;

    // store position in transform (we only store in Camera struct view later)
    // To generate view matrix we'll use LookAt each frame using current pos and front
    // Keep a local position store using camera.view[12..14]? We'll keep an extra temp here
    // Attach as part of camera via a simple known slot: we don't have explicit position field; use a private capture here.

    // Compose initial view
    float eyePos[3] = {eye[0], eye[1], eye[2]};
    LookAt(eyePos, center, up, m_camera.view);

    // default projection
    UpdateProjectionMatrix();
}

void CameraController::SetViewport(int w, int h) {
    m_width = w; m_height = h;
    UpdateProjectionMatrix();
}

void CameraController::UpdateProjectionMatrix() {
    float aspect = (m_height == 0) ? 16.0f/9.0f : static_cast<float>(m_width) / static_cast<float>(m_height);
    Perspective(60.0f, aspect, 0.1f, 1000.0f, m_camera.projection);
}

void CameraController::Update(float dt) {
    // Maintain an internal position and angles; we compute front from yaw/pitch
    static float pos[3] = {0.0f, 10.0f, 25.0f};
    const float worldUp[3] = {0.0f, 1.0f, 0.0f};

    // Mouse rotation (active only when input system has captured the mouse via right-click press)
    double mdx=0, mdy=0;
    if (InputSystem::IsMouseCaptured()) {
        InputSystem::GetMouseDelta(mdx, mdy);
        if (mdx != 0.0 || mdy != 0.0) {
            // invert y for natural look
            m_yaw += static_cast<float>(mdx) * m_sensitivity;
            m_pitch -= static_cast<float>(mdy) * m_sensitivity;
            if (m_pitch > 89.0f) m_pitch = 89.0f;
            if (m_pitch < -89.0f) m_pitch = -89.0f;
        }
    }

    // compute forward vector
    float yawRad = DegToRad(m_yaw);
    float pitchRad = DegToRad(m_pitch);
    float front[3];
    front[0] = std::cos(pitchRad) * std::cos(yawRad);
    front[1] = std::sin(pitchRad);
    front[2] = std::cos(pitchRad) * std::sin(yawRad);
    Normalize(front);

    // right vector
    float right[3];
    Cross(front, worldUp, right);
    Normalize(right);

    // movement (WASD + QE up/down)
    float velocity = m_speed * dt;
    float prevPos[3] = { pos[0], pos[1], pos[2] };
    if (InputSystem::IsKeyDown(GLFW_KEY_W)) {
        pos[0] += front[0] * velocity; pos[1] += front[1] * velocity; pos[2] += front[2] * velocity;
    }
    if (InputSystem::IsKeyDown(GLFW_KEY_S)) {
        pos[0] -= front[0] * velocity; pos[1] -= front[1] * velocity; pos[2] -= front[2] * velocity;
    }
    if (InputSystem::IsKeyDown(GLFW_KEY_A)) {
        pos[0] -= right[0] * velocity; pos[1] -= right[1] * velocity; pos[2] -= right[2] * velocity;
    }
    if (InputSystem::IsKeyDown(GLFW_KEY_D)) {
        pos[0] += right[0] * velocity; pos[1] += right[1] * velocity; pos[2] += right[2] * velocity;
    }
    if (InputSystem::IsKeyDown(GLFW_KEY_Q)) {
        pos[1] -= velocity;
    }
    if (InputSystem::IsKeyDown(GLFW_KEY_E)) {
        pos[1] += velocity;
    }


    // compute center point for look-at
    float center[3] = { pos[0] + front[0], pos[1] + front[1], pos[2] + front[2] };
    LookAt(pos, center, worldUp, m_camera.view);
}


