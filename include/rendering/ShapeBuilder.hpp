#pragma once

#include <memory>

class Mesh;

namespace ShapeBuilder {
    // Create a simple colored triangle (positions in NDC)
    std::unique_ptr<Mesh> CreateTriangle();

    // Create a simple colored quad (two triangles)
    std::unique_ptr<Mesh> CreateQuad();

    // Create a box (cube) - useful for car body, seats, etc.
    // Centered at origin, scale from -0.5 to +0.5 in each dimension
    // vertex format: pos.x pos.y pos.z  tex.u tex.v
    std::unique_ptr<Mesh> CreateBox();
}
