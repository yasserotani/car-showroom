#pragma once

struct GLFWwindow;

class InputSystem {
public:
    // Initialize with a GLFW window pointer (must be called after window creation)
    static void Init(GLFWwindow* window);
    static void PollEvents();

    // Query helpers
    static bool IsKeyDown(int key);
    static bool IsMouseButtonDown(int button);
    // Returns mouse delta since last PollEvents (dx, dy). Positive dx = right, positive dy = up
    static void GetMouseDelta(double& dx, double& dy);

    // Whether mouse is currently captured for camera rotation (enabled via right-click press)
    static bool IsMouseCaptured();
};
