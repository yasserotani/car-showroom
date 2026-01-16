#pragma once

#include "core/Renderable.hpp"
#include <memory>

class Shader;
class Mesh;
class Texture;

class FloorRenderable : public Renderable {
public:
    FloorRenderable(float px, float py, float pz, float sx, float sy, float sz);
    ~FloorRenderable();

    void RenderOpaque(const Camera& camera) override;
    void RenderTransparent(const Camera& camera) override;

private:
    std::unique_ptr<Shader> m_shader;
    std::unique_ptr<Mesh> m_mesh;
    Texture* m_texture = nullptr;
};
