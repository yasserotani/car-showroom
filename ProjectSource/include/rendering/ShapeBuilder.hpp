#pragma once

#include <memory>

class Mesh;

namespace ShapeBuilder {
    // Create a simple colored triangle (positions in NDC)
    std::unique_ptr<Mesh> CreateTriangle();

    // Create a simple colored quad (two triangles)
    std::unique_ptr<Mesh> CreateQuad();
}
