#include "rendering/Renderer.hpp"
#include "rendering/Mesh.hpp"
#include "rendering/Shader.hpp"
#include "core/Camera.hpp"
#include <glad/glad.h>

// internal camera state
static Camera s_camera;
static bool s_hasCamera = false;

void Renderer::Init() {
    glEnable(GL_DEPTH_TEST);
}

void Renderer::Clear() {
    glClearColor(0.1f, 0.1f, 0.15f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void Renderer::Begin() {
    // modern pipeline doesn't use matrix stacks
}

void Renderer::End() {
}

void Renderer::EnableDepth(bool on) {
    if (on) glEnable(GL_DEPTH_TEST);
    else glDisable(GL_DEPTH_TEST);
}

void Renderer::EnableBlending(bool on) {
    if (on) {
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    } else {
        glDisable(GL_BLEND);
    }
}

void Renderer::SetCamera(const Camera& cam) {
    s_camera = cam;
    s_hasCamera = true;
}

void Renderer::ClearCamera() {
    s_hasCamera = false;
}

bool Renderer::HasCamera() {
    return s_hasCamera;
}

void Renderer::ApplyCameraToShader(Shader& shader) {
    if (!s_hasCamera) return;
    shader.SetUniformMat4("uView", s_camera.view);
    shader.SetUniformMat4("uProj", s_camera.projection);
}

void Renderer::DrawMesh(const Mesh& mesh, const Shader& shader) {
    // Use provided shader then draw the mesh
    Shader& nsh = const_cast<Shader&>(shader);
    nsh.Use();

    // set common camera uniforms if present
    if (s_hasCamera) {
        nsh.SetUniformMat4("uView", s_camera.view);
        nsh.SetUniformMat4("uProj", s_camera.projection);
    }

    mesh.Draw();
}
