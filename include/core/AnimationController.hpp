#pragma once

class AnimationController {
public:
    AnimationController() = default;
    ~AnimationController() = default;

    void Init();
    void Update(float dt);
};
