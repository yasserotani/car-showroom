#include "core/InputSystem.hpp"
#include <GLFW/glfw3.h>

static GLFWwindow* s_window = nullptr;
static double s_lastX = 0.0, s_lastY = 0.0;
static double s_dx = 0.0, s_dy = 0.0;
static bool s_firstMouse = true;
static bool s_isCapturingMouse = false;
static int s_prevRightState = GLFW_RELEASE;

void InputSystem::Init(GLFWwindow* window) {
    s_window = window;
    // Initialize mouse state
    if (s_window) {
        double x, y;
        glfwGetCursorPos(s_window, &x, &y);
        s_lastX = x; s_lastY = y;
        s_firstMouse = false;
        // ensure cursor visible initially
        glfwSetInputMode(s_window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
        s_isCapturingMouse = false;
        s_prevRightState = GLFW_RELEASE;
    }
}

void InputSystem::PollEvents() {
    // Let GLFW process events
    glfwPollEvents();

    if (!s_window) return;

    // Check for right-button press to start capture mode (cursor disabled).
    int rightState = glfwGetMouseButton(s_window, GLFW_MOUSE_BUTTON_RIGHT);
    double x, y;
    glfwGetCursorPos(s_window, &x, &y);

    if (rightState == GLFW_PRESS && s_prevRightState != GLFW_PRESS) {
        // newly pressed -> enter capture mode
        s_isCapturingMouse = true;
        glfwSetInputMode(s_window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
        s_firstMouse = true; // reset reference so first delta won't jump
    }

    // Exit capture mode upon ESC (explicit user action)
    if (glfwGetKey(s_window, GLFW_KEY_ESCAPE) == GLFW_PRESS && s_isCapturingMouse) {
        s_isCapturingMouse = false;
        glfwSetInputMode(s_window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
        s_dx = 0.0; s_dy = 0.0;
        s_firstMouse = true;
    }

    if (s_isCapturingMouse) {
        if (s_firstMouse) {
            s_lastX = x; s_lastY = y;
            s_firstMouse = false;
            s_dx = 0.0; s_dy = 0.0;
        } else {
            s_dx = x - s_lastX;
            s_dy = y - s_lastY;
        }
    } else {
        // not capturing -> no rotation deltas
        s_dx = 0.0; s_dy = 0.0;
        s_firstMouse = true;
    }

    s_lastX = x; s_lastY = y;
    s_prevRightState = rightState;
}

bool InputSystem::IsMouseCaptured() {
    return s_isCapturingMouse;
} 

bool InputSystem::IsKeyDown(int key) {
    if (!s_window) return false;
    return glfwGetKey(s_window, key) == GLFW_PRESS;
}

bool InputSystem::IsMouseButtonDown(int button) {
    if (!s_window) return false;
    return glfwGetMouseButton(s_window, button) == GLFW_PRESS;
}

void InputSystem::GetMouseDelta(double& dx, double& dy) {
    dx = s_dx; dy = s_dy;
}

