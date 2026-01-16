#pragma once

#include "core/Renderable.hpp"
#include <memory>
#include <string>

class Shader;
class Mesh;
class Texture;

class WallRenderable : public Renderable {
public:
    WallRenderable(float px, float py, float pz,
                float sx, float sy, float sz,
                const std::string& texturePath,
                bool transparent = false);

    ~WallRenderable();

    void RenderOpaque(const Camera& camera) override;
    void RenderTransparent(const Camera& camera) override;

private:
    std::unique_ptr<Shader> m_shader;
    std::unique_ptr<Mesh> m_mesh;
    Texture* m_texture = nullptr;
    bool m_isTransparent = false;
};
