// include/core/SceneManager.hpp
#pragma once

#include <vector>
#include <memory>
#include <string>
#include "core/Renderable.hpp"

class CameraController;
class AnimationController;
class HallBase;

class SceneManager {
public:
    SceneManager();
    ~SceneManager();

    // Initialize registrars, camera, halls and cars
    void Init();

    // Update scene logic (called every frame)
    void Update(float dt);

    // Render opaque objects (first pass)
    void RenderOpaque();

    // Render transparent objects (second pass)
    void RenderTransparent();

    // Convenience full-frame render (opaque then transparent)
    void Render();

    // Cleanup resources
    void Shutdown();

    // Helpers
    void SetActiveHall(const std::string& name);
    HallBase* GetActiveHall() const;

private:
    std::unique_ptr<CameraController> m_camera;
    std::unique_ptr<AnimationController> m_anim;

    // active hall pointer (owned by HallRegistrar)
    HallBase* m_activeHall;

    // simple timing / state
    bool m_initialized;

    // Owned renderables
    std::vector<std::unique_ptr<class Renderable>> m_opaqueObjects;
    std::vector<std::unique_ptr<class Renderable>> m_transparentObjects;
};
