#pragma once

#include "core/SceneManager.hpp"
struct GLFWwindow;

class App {
public:
    App();
    void Run();

private:
    SceneManager m_Scene;
    GLFWwindow* m_Window = nullptr;
};
