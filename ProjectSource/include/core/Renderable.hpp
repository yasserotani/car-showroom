#pragma once

#include "core/Camera.hpp"

// Simple transform: position, rotation (euler), scale
struct Transform {
    float position[3] = {0.0f, 0.0f, 0.0f};
    float rotation[3] = {0.0f, 0.0f, 0.0f};
    float scale[3]    = {1.0f, 1.0f, 1.0f};
};

class Renderable {
public:
    virtual ~Renderable() = default;

    Transform transform;

    // Render passes
    virtual void RenderOpaque(const Camera& camera) = 0;
    virtual void RenderTransparent(const Camera& camera) = 0;
};
