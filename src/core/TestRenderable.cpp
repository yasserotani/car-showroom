#include "core/TestRenderable.hpp"
#include "rendering/Shader.hpp"
#include "rendering/Mesh.hpp"
#include "rendering/ShapeBuilder.hpp"
#include "rendering/Renderer.hpp"
#include <iostream>
#include <filesystem>
#include <fstream>

TestRenderable::TestRenderable() {}
TestRenderable::~TestRenderable() {}

bool TestRenderable::Init() {
    const char* vs = R"(
        #version 330 core
        layout(location = 0) in vec3 aPos;
        layout(location = 1) in vec3 aColor;
        out vec3 vColor;
        void main() {
            vColor = aColor;
            gl_Position = vec4(aPos, 1.0);
        }
    )";

    const char* fs = R"(
        #version 330 core
        in vec3 vColor;
        out vec4 FragColor;
        void main() {
            FragColor = vec4(vColor, 1.0);
        }
    )";

    m_shader = std::make_unique<Shader>();
    if (!m_shader->CompileFromSource(vs, fs)) {
        std::cerr << "TestRenderable: shader compilation failed\n";
        m_shader.reset();
        return false;
    }

    m_mesh = ShapeBuilder::CreateTriangle();
    if (!m_mesh) {
        std::cerr << "TestRenderable: mesh creation failed\n";
        return false;
    }

    // final OK
    {
        // init OK
    }

    return true;
}

void TestRenderable::RenderOpaque(const Camera& camera) {
    if (!m_shader || !m_mesh) return;

    // set matrices
    float identity[16] = {1,0,0,0, 0,1,0,0, 0,0,1,0, 0,0,0,1};
    m_shader->Use();
    m_shader->SetUniformMat4("uModel", identity);
    m_shader->SetUniformMat4("uView", camera.view);
    m_shader->SetUniformMat4("uProj", camera.projection);

    Renderer::EnableDepth(false);
    Renderer::EnableBlending(false);
    Renderer::DrawMesh(*m_mesh, *m_shader);

    if (!m_logged) {
        m_logged = true;
    }
}

void TestRenderable::RenderTransparent(const Camera& /*camera*/) {
    // Nothing to render for transparent pass in this test
}
