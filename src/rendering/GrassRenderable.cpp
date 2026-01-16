#include "rendering/GrassRenderable.hpp"
#include "rendering/Shader.hpp"
#include "rendering/Mesh.hpp"
#include "rendering/ShapeBuilder.hpp"
#include "rendering/Texture.hpp"
#include "rendering/Renderer.hpp"

#include <iostream>

GrassRenderable::GrassRenderable() {
    // Load shader
    m_shader = std::make_unique<Shader>();
    if (!m_shader->CompileFromFiles("shaders/basic_texture.vert", "shaders/basic_texture.frag")) {
        std::cerr << "GrassRenderable: shader compile failed\n";
        m_shader.reset();
        return;
    }

    // Create quad mesh (XZ plane)
    m_mesh = ShapeBuilder::CreateQuad();
    if (!m_mesh) {
        std::cerr << "GrassRenderable: mesh creation failed\n";
        return;
    }

    // Load grass texture
    m_texture = Texture::LoadFromFile("textures/grass.png");
    if (!m_texture) {
        std::cerr << "GrassRenderable: failed to load assets/textures/grass.png, using fallback\n";
        m_texture = Texture::CreateSolidColor(34, 139, 34, 255); // fallback green
    }

    if (!m_texture) {
        std::cerr << "GrassRenderable: failed to create fallback texture\n";
        return;
    }

    // Move grass away from the road so we can see it clearly
    m_transform.position[0] = -100.0f; // X
    m_transform.position[1] = 0.0f;    // Y
    m_transform.position[2] = 0.0f;    // Z

    // Reasonable size
    m_transform.scale[0] = 50.0f;
    m_transform.scale[1] = 1.0f;
    m_transform.scale[2] = 50.0f;

    // Sync with base class
    transform = m_transform;
}

GrassRenderable::~GrassRenderable() {
    if (m_texture) {
        m_texture->Destroy();
        delete m_texture;
        m_texture = nullptr;
    }
}

static void BuildModelMatrixFromTransform(const Transform& t, float out[16]) {
    // Column-major matrix: scale then translation (no rotation)
    out[0] = t.scale[0]; out[4] = 0.0f;       out[8]  = 0.0f;       out[12] = t.position[0];
    out[1] = 0.0f;       out[5] = t.scale[1]; out[9]  = 0.0f;       out[13] = t.position[1];
    out[2] = 0.0f;       out[6] = 0.0f;       out[10] = t.scale[2]; out[14] = t.position[2];
    out[3] = 0.0f;       out[7] = 0.0f;       out[11] = 0.0f;       out[15] = 1.0f;
}

void GrassRenderable::RenderOpaque(const Camera& camera) {
    if (!m_shader || !m_mesh || !m_texture) return;

    m_shader->Use();

    float model[16];
    BuildModelMatrixFromTransform(m_transform, model);
    m_shader->SetUniformMat4("uModel", model);
    m_shader->SetUniformMat4("uView", camera.view);
    m_shader->SetUniformMat4("uProj", camera.projection);

    // Bind texture
    m_shader->SetUniformInt("uTexture", 0);
    m_texture->Bind(0);

    Renderer::EnableDepth(true);
    Renderer::EnableBlending(false);
    Renderer::DrawMesh(*m_mesh, *m_shader);
}

void GrassRenderable::RenderTransparent(const Camera&) {
    // Nothing to render in transparent pass
}
