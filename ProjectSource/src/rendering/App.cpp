#include "rendering/App.hpp"

// NOTE: The following includes may trigger VS Code IntelliSense errors in some setups
// (false positives). They are required for the build and compile correctly via CMake.
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "rendering/Renderer.hpp"
#include "core/InputSystem.hpp"
#include <filesystem>
#include <fstream>
#include <iostream>

App::App() {}

void App::Run() {

    // 1. Initialize GLFW
    if (!glfwInit()) {
        std::cerr << "Failed to initialize GLFW\n";
        return;
    }

    // 2. Set OpenGL version (3.3 Core)
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // 3. Create window
    m_Window = glfwCreateWindow(1280, 720, "Car Showroom 2025", nullptr, nullptr);
    if (!m_Window) {
        std::cerr << "Failed to create GLFW window\n";
        glfwTerminate();
        return;
    }

    glfwMakeContextCurrent(m_Window);

    // 4. Load OpenGL functions via GLAD
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cerr << "Failed to initialize GLAD\n";
        return;
    }

    // 5. Set viewport
    glViewport(0, 0, 1280, 720);

    // 6. Initialize input system and scene (must be after GL context is ready)
    InputSystem::Init(m_Window);
    m_Scene.Init();
    // Scene initialized

    // simple timing
    double lastTime = glfwGetTime();

    // 7. Main loop
    while (!glfwWindowShouldClose(m_Window)) {
        // Poll OS/window events
        glfwPollEvents();

        // compute delta time
        double now = glfwGetTime();
        float dt = static_cast<float>(now - lastTime);
        lastTime = now;

        // Update scene logic
        m_Scene.Update(dt);

        // Clear buffers
        Renderer::Clear();

        // Frame render
        m_Scene.Render();

        // Swap buffers
        glfwSwapBuffers(m_Window);
    }

    // 8. Shutdown scene
    m_Scene.Shutdown();

    // 9. Cleanup
    glfwDestroyWindow(m_Window);
    glfwTerminate();
}
