#include "rendering/ShapeBuilder.hpp"
#include "rendering/Mesh.hpp"
#include <memory>

namespace ShapeBuilder {

std::unique_ptr<Mesh> CreateTriangle() {
    // Positions in NDC range, with per-vertex color
    // vertex format: pos.x pos.y pos.z  col.r col.g col.b
    std::vector<float> verts = {
        // triangle
        0.0f, 0.0f,  0.5f, 0.0f,   1.0f, 0.0f, 0.0f,
        -0.5f, -0.5f, 0.0f,   0.0f, 1.0f, 0.0f,
        0.5f, -0.5f, 0.0f,   0.0f, 0.0f, 1.0f
    };
    std::vector<unsigned int> indices; // not needed
    return std::make_unique<Mesh>(verts, indices, 6);
}

std::unique_ptr<Mesh> CreateQuad() {
    // Quad on XZ plane (y = 0), facing UP (towards +Y)
    // vertex format: pos.x pos.y pos.z  tex.u tex.v
    std::vector<float> verts = {
        // positions            // texcoords
        -0.5f, 0.0f, -0.5f,     0.0f, 0.0f,  // bottom-left
         0.5f, 0.0f, -0.5f,     1.0f, 0.0f,  // bottom-right
         0.5f, 0.0f,  0.5f,     1.0f, 1.0f,  // top-right
        -0.5f, 0.0f,  0.5f,     0.0f, 1.0f   // top-left
    };

    // IMPORTANT: winding order is CCW so the quad faces UP
    std::vector<unsigned int> idx = {0, 1, 2, 0, 2, 3};

    return std::make_unique<Mesh>(verts, idx, 5);
}


} // namespace ShapeBuilder