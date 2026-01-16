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

std::unique_ptr<Mesh> CreateBox() {
    // Box centered at origin, from -0.5 to +0.5 in each dimension
    // vertex format: pos.x pos.y pos.z  tex.u tex.v
    // We'll create 6 faces (front, back, left, right, top, bottom)
    std::vector<float> verts;
    std::vector<unsigned int> indices;

    // Front face (Z = +0.5)
    unsigned int frontStart = 0;
    verts.insert(verts.end(), {-0.5f, -0.5f,  0.5f,  0.0f, 0.0f});
    verts.insert(verts.end(), { 0.5f, -0.5f,  0.5f,  1.0f, 0.0f});
    verts.insert(verts.end(), { 0.5f,  0.5f,  0.5f,  1.0f, 1.0f});
    verts.insert(verts.end(), {-0.5f,  0.5f,  0.5f,  0.0f, 1.0f});
    indices.insert(indices.end(), {frontStart+0, frontStart+1, frontStart+2, frontStart+0, frontStart+2, frontStart+3});

    // Back face (Z = -0.5)
    unsigned int backStart = 4;
    verts.insert(verts.end(), { 0.5f, -0.5f, -0.5f,  0.0f, 0.0f});
    verts.insert(verts.end(), {-0.5f, -0.5f, -0.5f,  1.0f, 0.0f});
    verts.insert(verts.end(), {-0.5f,  0.5f, -0.5f,  1.0f, 1.0f});
    verts.insert(verts.end(), { 0.5f,  0.5f, -0.5f,  0.0f, 1.0f});
    indices.insert(indices.end(), {backStart+0, backStart+1, backStart+2, backStart+0, backStart+2, backStart+3});

    // Left face (X = -0.5)
    unsigned int leftStart = 8;
    verts.insert(verts.end(), {-0.5f, -0.5f, -0.5f,  0.0f, 0.0f});
    verts.insert(verts.end(), {-0.5f, -0.5f,  0.5f,  1.0f, 0.0f});
    verts.insert(verts.end(), {-0.5f,  0.5f,  0.5f,  1.0f, 1.0f});
    verts.insert(verts.end(), {-0.5f,  0.5f, -0.5f,  0.0f, 1.0f});
    indices.insert(indices.end(), {leftStart+0, leftStart+1, leftStart+2, leftStart+0, leftStart+2, leftStart+3});

    // Right face (X = +0.5)
    unsigned int rightStart = 12;
    verts.insert(verts.end(), { 0.5f, -0.5f,  0.5f,  0.0f, 0.0f});
    verts.insert(verts.end(), { 0.5f, -0.5f, -0.5f,  1.0f, 0.0f});
    verts.insert(verts.end(), { 0.5f,  0.5f, -0.5f,  1.0f, 1.0f});
    verts.insert(verts.end(), { 0.5f,  0.5f,  0.5f,  0.0f, 1.0f});
    indices.insert(indices.end(), {rightStart+0, rightStart+1, rightStart+2, rightStart+0, rightStart+2, rightStart+3});

    // Top face (Y = +0.5)
    unsigned int topStart = 16;
    verts.insert(verts.end(), {-0.5f,  0.5f,  0.5f,  0.0f, 0.0f});
    verts.insert(verts.end(), { 0.5f,  0.5f,  0.5f,  1.0f, 0.0f});
    verts.insert(verts.end(), { 0.5f,  0.5f, -0.5f,  1.0f, 1.0f});
    verts.insert(verts.end(), {-0.5f,  0.5f, -0.5f,  0.0f, 1.0f});
    indices.insert(indices.end(), {topStart+0, topStart+1, topStart+2, topStart+0, topStart+2, topStart+3});

    // Bottom face (Y = -0.5)
    unsigned int bottomStart = 20;
    verts.insert(verts.end(), {-0.5f, -0.5f, -0.5f,  0.0f, 0.0f});
    verts.insert(verts.end(), { 0.5f, -0.5f, -0.5f,  1.0f, 0.0f});
    verts.insert(verts.end(), { 0.5f, -0.5f,  0.5f,  1.0f, 1.0f});
    verts.insert(verts.end(), {-0.5f, -0.5f,  0.5f,  0.0f, 1.0f});
    indices.insert(indices.end(), {bottomStart+0, bottomStart+1, bottomStart+2, bottomStart+0, bottomStart+2, bottomStart+3});

    return std::make_unique<Mesh>(verts, indices, 5);
}

} // namespace ShapeBuilder