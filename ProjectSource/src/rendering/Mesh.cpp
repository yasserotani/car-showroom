#include "rendering/Mesh.hpp"
#include <glad/glad.h>

Mesh::Mesh(const std::vector<float>& vertices, const std::vector<unsigned int>& indices, unsigned int vertexStride)
    : m_vertexCount(static_cast<int>(vertices.size() / vertexStride))
    , m_indexCount(static_cast<int>(indices.size()))
    , m_vertexStride(vertexStride)
    , m_hasIndices(!indices.empty())
{
    glGenVertexArrays(1, &m_vao);
    glGenBuffers(1, &m_vbo);
    glBindVertexArray(m_vao);

    glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), vertices.data(), GL_STATIC_DRAW);

    if (m_hasIndices) {
        glGenBuffers(1, &m_ebo);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ebo);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), indices.data(), GL_STATIC_DRAW);
    }

    // We support two common layouts:
    // - position (vec3) + color (vec3)  -> vertexStride == 6
    // - position (vec3) + texcoord (vec2) -> vertexStride == 5
    // stride in bytes
    GLsizei strideBytes = static_cast<GLsizei>(m_vertexStride * sizeof(float));
    // position (location 0)
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, strideBytes, reinterpret_cast<void*>(0));

    // attribute at location 1: either color (vec3) or texcoord (vec2)
    if (m_vertexStride == 6) {
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, strideBytes, reinterpret_cast<void*>(3 * sizeof(float)));
    } else if (m_vertexStride == 5) {
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, strideBytes, reinterpret_cast<void*>(3 * sizeof(float)));
    }

    // unbind
    glBindVertexArray(0);
}

Mesh::~Mesh() {
    if (m_ebo) glDeleteBuffers(1, &m_ebo);
    if (m_vbo) glDeleteBuffers(1, &m_vbo);
    if (m_vao) glDeleteVertexArrays(1, &m_vao);
}

void Mesh::Draw() const {
    glBindVertexArray(m_vao);
    if (m_hasIndices) {
        glDrawElements(GL_TRIANGLES, m_indexCount, GL_UNSIGNED_INT, nullptr);
    } else {
        glDrawArrays(GL_TRIANGLES, 0, m_vertexCount);
    }
    glBindVertexArray(0);
}