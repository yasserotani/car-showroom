#pragma once

#include "core/Renderable.hpp"
#include <memory>

class Shader;
class Mesh;

class TestRenderable : public Renderable {
public:
    TestRenderable();
    ~TestRenderable();

    // Build GPU resources (shader/mesh)
    bool Init();

    void RenderOpaque(const Camera& camera) override;
    void RenderTransparent(const Camera& camera) override;

private:
    std::unique_ptr<Shader> m_shader;
    std::unique_ptr<Mesh> m_mesh;

    // debug
    bool m_logged = false;
};
