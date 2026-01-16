#pragma once

// Minimal Camera struct used by Renderable interface.
// For now it exposes raw 4x4 matrices as float arrays (column-major)
// and provides a default identity initialization.
struct Camera {
    float view[16];
    float projection[16];

    Camera() {
        for (int i = 0; i < 16; ++i) {
            view[i] = 0.0f;
            projection[i] = 0.0f;
        }
        view[0] = view[5] = view[10] = view[15] = 1.0f;
        projection[0] = projection[5] = projection[10] = projection[15] = 1.0f;
    }
};
