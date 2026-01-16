#include "rendering/FloorRenderable.hpp"
#include "rendering/Shader.hpp"
#include "rendering/Mesh.hpp"
#include "rendering/ShapeBuilder.hpp"
#include "rendering/Texture.hpp"
#include "rendering/Renderer.hpp"

#include <iostream>
#include <filesystem>
#include <fstream>

FloorRenderable::FloorRenderable(float px, float py, float pz, float sx, float sy, float sz) {
    m_shader = std::make_unique<Shader>();
    if (!m_shader->CompileFromFiles("shaders/basic_texture.vert", "shaders/basic_texture.frag")) {
        std::cerr << "FloorRenderable: shader compile failed\n";
        m_shader.reset();
        return;
    } else {
        // shader compiled OK
    }

    m_mesh = ShapeBuilder::CreateQuad();
    if (!m_mesh) {
        std::cerr << "FloorRenderable: mesh creation failed\n";
    } else {
        // mesh created OK
    }

    // Try common runtime paths for tile texture
    std::vector<std::string> candidates = {
        "textures/tile_floor.png",
        "assets/textures/tile_floor.png",
        "build/Debug/textures/tile_floor.png",
        "build/textures/tile_floor.png"
    };
    for (const auto& c : candidates) {
        m_texture = Texture::LoadFromFile(c);
        if (m_texture) {
            break;
        }
    }
    if (!m_texture) {
        std::cerr << "FloorRenderable: failed to load tile_floor.png, creating fallback\n";
        m_texture = Texture::CreateSolidColor(200, 200, 200, 255);
    }

    // set transform
    transform.position[0] = px; transform.position[1] = py; transform.position[2] = pz;
    transform.scale[0] = sx+10; transform.scale[1] = sy; transform.scale[2] = sz+10;
}

FloorRenderable::~FloorRenderable() {
    if (m_texture) {
        m_texture->Destroy();
        delete m_texture;
        m_texture = nullptr;
    }
}

static void BuildModelMatrix(const Transform& t, float out[16]) {
    out[0] = t.scale[0]; out[4] = 0.0f;       out[8]  = 0.0f;       out[12] = t.position[0];
    out[1] = 0.0f;       out[5] = t.scale[1]; out[9]  = 0.0f;       out[13] = t.position[1];
    out[2] = 0.0f;       out[6] = 0.0f;       out[10] = t.scale[2]; out[14] = t.position[2];
    out[3] = 0.0f;       out[7] = 0.0f;       out[11] = 0.0f;       out[15] = 1.0f;
}

void FloorRenderable::RenderOpaque(const Camera& camera) {
    if (!m_shader || !m_mesh || !m_texture) return;

    m_shader->Use();

    float model[16];
    BuildModelMatrix(transform, model);
    m_shader->SetUniformMat4("uModel", model);

    // Apply camera matrices from Renderer if available (Renderer holds global camera state)
    Renderer::ApplyCameraToShader(*m_shader);

    m_shader->SetUniformInt("uTexture", 0);
    m_texture->Bind(0);

    Renderer::EnableDepth(true);
    Renderer::EnableBlending(false);
    Renderer::DrawMesh(*m_mesh, *m_shader);
}

void FloorRenderable::RenderTransparent(const Camera& /*camera*/) {
    // nothing
}
