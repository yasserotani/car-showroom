// src/core/SceneManager.cpp
#include "core/SceneManager.hpp"
#include "core/CameraController.hpp"
#include "core/AnimationController.hpp"
#include "registrars/CarRegistrar.hpp"
#include "registrars/HallRegistrar.hpp"
#include "rendering/Renderer.hpp"
#include "rendering/ShaderRegistrar.hpp"
#include "rendering/TextureRegistrar.hpp"
#include "rendering/ModelRegistrar.hpp"
#include "rendering/Shader.hpp"
#include "rendering/Mesh.hpp"
#include "rendering/ShapeBuilder.hpp"
#include "core/InputSystem.hpp"
#include "halls/HallBase.hpp"

#include <iostream>
#include <fstream>
#include <filesystem>

SceneManager::SceneManager()
    : m_camera(nullptr)
    , m_anim(nullptr)
    , m_activeHall(nullptr)
    , m_initialized(false)
{
}

SceneManager::~SceneManager() {
    Shutdown();
}

void SceneManager::Init() {
    if (m_initialized) return;

    // Quick runtime trace for tests
    try {
        auto cwdLog = std::filesystem::current_path() / "shader_test_started.txt";
        std::ofstream started(cwdLog.string(), std::ios::app);
        if (started) started << "SceneManager::Init invoked\n";
    } catch (...) { }


    // Initialize rendering subsystems and resource registrars
    Renderer::Init();
    ShaderRegistrar::Init();
    TextureRegistrar::Init();
    ModelRegistrar::Init();

    // Initialize domain registrars (cars and halls)
    CarRegistrar::Init();
    HallRegistrar::Init();

    // Create camera and animation controller
    m_camera = std::make_unique<CameraController>();
    m_camera->Init();

    m_anim = std::make_unique<AnimationController>();
    m_anim->Init();

    // Set default active hall to family (the complex car)
    m_activeHall = HallRegistrar::Get("family");
    if (!m_activeHall) {
        // fallback to any available hall
        m_activeHall = HallRegistrar::Get("sport");
    }

    // Minimal triangle demo: compile a basic shader and create a triangle mesh
    // Simple vertex shader: position (location 0), color (location 1) -> gl_Position, out color
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

    // --------------- Shader compile tests (minimal) ---------------
    {
        auto testCompile = [&](const std::string& vs, const std::string& fs, const std::string& name) {
            Shader s;
            bool ok = s.CompileFromFiles(vs, fs);
            if (ok) {
                std::cerr << "SceneManager: shader '" << name << "' compiled OK\n";
            } else {
                std::cerr << "SceneManager: shader '" << name << "' failed to compile\n";
            }
        };

        testCompile("shaders/basic_texture.vert", "shaders/basic_texture.frag", "BasicTexture");
        testCompile("shaders/car_lighting.vert", "shaders/car_lighting.frag", "CarLighting");
        testCompile("shaders/transparent.vert", "shaders/transparent.frag", "Transparent");
    }

    // Test renderable disabled: triangle demo removed (will be added later via scene objects)

    // NOTE: GrassRenderable removed; floors are now owned by Halls
    // (FloorRenderable instances are created inside each hall's Init())

    m_initialized = true;
}

void SceneManager::Update(float dt) {
    if (!m_initialized) return;

    // Poll input events
    InputSystem::PollEvents();

    // Update camera and animations
    if (m_camera) m_camera->Update(dt);
    if (m_anim) m_anim->Update(dt);

    // Update active hall and its cars
    if (m_activeHall) m_activeHall->Update(dt);
}

void SceneManager::RenderOpaque() {
    if (!m_initialized) return;

    // Render all opaque renderables
    for (auto& obj : m_opaqueObjects) {
        if (obj) {
            if (m_camera) obj->RenderOpaque(m_camera->GetCamera());
            else obj->RenderOpaque(Camera());
        }
    }

    // Render hall geometry and non-transparent car parts
    // Render all registered halls so their floors are visible simultaneously
    auto allHalls = HallRegistrar::GetAll();
    for (auto* h : allHalls) {
        if (h) h->RenderOpaque();
    }
}

void SceneManager::RenderTransparent() {
    if (!m_initialized) return;

    // Render transparent renderables
    for (auto& obj : m_transparentObjects) {
        if (obj) {
            if (m_camera) obj->RenderTransparent(m_camera->GetCamera());
            else obj->RenderTransparent(Camera());
        }
    }

    // Render transparent materials like glass from the hall
    if (m_activeHall) m_activeHall->RenderTransparent();
}

void SceneManager::Shutdown() {
    if (!m_initialized) return;

    // Destroy halls and cars if registrars don't own lifetime
    // (In our design registrars own instances; if not, iterate and delete here)

    // Reset controllers
    if (m_anim) { m_anim.reset(); }
    if (m_camera) { m_camera.reset(); }

    // Destroy owned renderables first
    m_transparentObjects.clear();
    m_opaqueObjects.clear();

    // Shutdown registrars (reverse of init)
    ModelRegistrar::Shutdown();
    TextureRegistrar::Shutdown();
    ShaderRegistrar::Shutdown();
    HallRegistrar::Shutdown();
    CarRegistrar::Shutdown();

    m_initialized = false;
}

void SceneManager::SetActiveHall(const std::string& name) {
    HallBase* h = HallRegistrar::Get(name);
    if (h) m_activeHall = h;
}

HallBase* SceneManager::GetActiveHall() const {
    return m_activeHall;
}

void SceneManager::Render() {
    if (!m_initialized) return;

    // Provide camera to renderer so it can set common shader uniforms like uView and uProj
    if (m_camera) Renderer::SetCamera(m_camera->GetCamera());
    else Renderer::ClearCamera();

    RenderOpaque();
    RenderTransparent();
}
