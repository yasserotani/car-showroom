#pragma once

#include <vector>

class Mesh {
public:
    // vertices: flat float array containing interleaved attributes (pos+color) per vertex
    Mesh(const std::vector<float>& vertices, const std::vector<unsigned int>& indices, unsigned int vertexStride);
    ~Mesh();

    // Draw (binds VAO and issues draw call)
    void Draw() const;

private:
    unsigned int m_vao = 0;
    unsigned int m_vbo = 0;
    unsigned int m_ebo = 0;

    int m_vertexCount = 0;
    int m_indexCount = 0;
    unsigned int m_vertexStride = 0; // in floats
    bool m_hasIndices = false;
};