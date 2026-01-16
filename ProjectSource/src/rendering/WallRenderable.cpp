#include "rendering/WallRenderable.hpp"
#include "rendering/Shader.hpp"
#include "rendering/Mesh.hpp"
#include "rendering/ShapeBuilder.hpp"
#include "rendering/Texture.hpp"
#include "rendering/Renderer.hpp"

#include <iostream>

static void BuildModelMatrix(const Transform& t, float out[16]) {
    out[0] = t.scale[0]; out[4] = 0.0f;       out[8]  = 0.0f;       out[12] = t.position[0];
    out[1] = 0.0f;       out[5] = t.scale[1]; out[9]  = 0.0f;       out[13] = t.position[1];
    out[2] = 0.0f;       out[6] = 0.0f;       out[10] = t.scale[2]; out[14] = t.position[2];
    out[3] = 0.0f;       out[7] = 0.0f;       out[11] = 0.0f;       out[15] = 1.0f;
}

WallRenderable::WallRenderable(float px, float py, float pz,
                            float sx, float sy, float sz,
                            const std::string& texturePath,
                            bool transparent)
{
    m_isTransparent = transparent;

    // Load shader
    m_shader = std::make_unique<Shader>();
    if (!m_shader->CompileFromFiles("shaders/basic_texture.vert", "shaders/basic_texture.frag")) {
        std::cerr << "WallRenderable: shader compile failed\n";
        return;
    }

    // Create quad mesh (vertical wall)
    m_mesh = ShapeBuilder::CreateQuad();

    // Load texture
    m_texture = Texture::LoadFromFile(texturePath);
    if (!m_texture) {
        std::cerr << "WallRenderable: failed to load texture, using fallback\n";
        m_texture = Texture::CreateSolidColor(200, 200, 200, 255);
    }

    // Set transform
    transform.position[0] = px;
    transform.position[1] = py;
    transform.position[2] = pz;

    transform.scale[0] = sx; // width
    transform.scale[1] = sy; // height
    transform.scale[2] = sz; // thickness
}

WallRenderable::~WallRenderable() {
    if (m_texture) {
        m_texture->Destroy();
        delete m_texture;
        m_texture = nullptr;
    }
}

void WallRenderable::RenderOpaque(const Camera& camera) {
    if (m_isTransparent) return;

    m_shader->Use();

    float model[16];
    BuildModelMatrix(transform, model);
    m_shader->SetUniformMat4("uModel", model);

    Renderer::ApplyCameraToShader(*m_shader);

    m_shader->SetUniformInt("uTexture", 0);
    m_texture->Bind(0);

    Renderer::EnableDepth(true);
    Renderer::EnableBlending(false);
    Renderer::DrawMesh(*m_mesh, *m_shader);
}

void WallRenderable::RenderTransparent(const Camera& camera) {
    if (!m_isTransparent) return;

    m_shader->Use();

    float model[16];
    BuildModelMatrix(transform, model);
    m_shader->SetUniformMat4("uModel", model);

    Renderer::ApplyCameraToShader(*m_shader);

    m_shader->SetUniformInt("uTexture", 0);
    m_texture->Bind(0);

    Renderer::EnableDepth(true);
    Renderer::EnableBlending(true);
    Renderer::DrawMesh(*m_mesh, *m_shader);
}
