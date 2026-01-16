#pragma once

#include "core/Renderable.hpp"
#include <memory>

class Shader;
class Mesh;
class Texture;

class GrassRenderable : public Renderable {
public:
    GrassRenderable();
    ~GrassRenderable();

    void RenderOpaque(const Camera& camera) override;
    void RenderTransparent(const Camera& camera) override;

private:
    std::unique_ptr<Shader> m_shader;
    std::unique_ptr<Mesh> m_mesh;
    Texture* m_texture = nullptr;
    Transform m_transform;
};
