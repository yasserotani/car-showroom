#pragma once

class Mesh;
class Shader;
struct Camera; // forward

class Renderer {
public:
    static void Init();
    static void Clear();
    static void Begin();
    static void End();

    static void EnableDepth(bool on);
    static void EnableBlending(bool on);

    // Camera passing so Renderer can set common uniforms
    static void SetCamera(const Camera& cam);
    static void ClearCamera();

    // Helper: whether renderer has camera state available
    static bool HasCamera();
    // Apply internally stored camera uniforms to a shader (called when shader is bound)
    static void ApplyCameraToShader(Shader& shader);

    // Draw a mesh with the currently bound shader (shader will be used if provided)
    static void DrawMesh(const Mesh& mesh, const Shader& shader);
};
