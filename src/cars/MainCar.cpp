#include "cars/MainCar.hpp"
#include "rendering/ShapeBuilder.hpp"
#include "rendering/Shader.hpp"
#include "rendering/Texture.hpp"
#include "rendering/Mesh.hpp"
#include "rendering/Renderer.hpp"
#include "core/Camera.hpp"
#include <cmath>
#include <iostream>
#include <vector>

MainCar::MainCar() { 
    m_name = "main";
    // Initialize position - place car in center hall
    m_position[0] = 0.0f;
    m_position[1] = 0.0f;
    m_position[2] = 0.0f;
    m_rotation = 0.0f;
}

MainCar::~MainCar() {
    Destroy();
}

void MainCar::Init() {
    CreateExterior();
    CreateInterior();
    CreateWheels();
    std::cerr << "MainCar: Initialized with " << m_opaqueComponents.size() 
              << " opaque and " << m_transparentComponents.size() << " transparent components\n";
}

void MainCar::Update(float dt) {
    (void)dt;
    // Future: animation updates (door opening, wheel rotation, etc.)
}

void MainCar::CreateExterior() {
    // Main car body - more realistic proportions (wider, lower, longer)
    {
        CarComponent body;
        body.mesh = ShapeBuilder::CreateBox();
        body.shader = std::make_unique<Shader>();
        if (!body.shader->CompileFromFiles("shaders/car_lighting.vert", "shaders/car_lighting.frag")) {
            body.shader->CompileFromFiles("shaders/basic_texture.vert", "shaders/basic_texture.frag");
        }
        
        body.texture = Texture::LoadFromFile("textures/wall_outside.png");
        if (!body.texture) {
            body.texture = Texture::CreateSolidColor(30, 50, 120, 255); // Darker blue car body
        }ew
        
        body.position[0] = 0.0f;
        body.position[1] = 0.5f; // Body center
        body.position[2] = 0.0f;
        body.scale[0] = 1.8f;  // width (wider)
        body.scale[1] = 0.6f;  // height (lower profile)
        body.scale[2] = 4.5f;  // length (longer)
        body.isTransparent = false;
        
        m_opaqueComponents.push_back(std::move(body));
    }

    // Car roof - more integrated look
    {
        CarComponent roof;
        roof.mesh = ShapeBuilder::CreateBox();
        roof.shader = std::make_unique<Shader>();
        if (!roof.shader->CompileFromFiles("shaders/basic_texture.vert", "shaders/basic_texture.frag")) {
            roof.shader.reset();
        }
        
        roof.texture = Texture::LoadFromFile("textures/roof.png");
        if (!roof.texture) {
            roof.texture = Texture::CreateSolidColor(20, 40, 100, 255); // Darker blue roof
        }
        
        roof.position[0] = 0.0f;
        roof.position[1] = 1.1f; // Above body
        roof.position[2] = -0.2f; // Slightly forward
        roof.scale[0] = 1.6f;  // Slightly narrower than body
        roof.scale[1] = 0.4f;  // Lower profile
        roof.scale[2] = 3.0f;   // Shorter than body
        roof.isTransparent = false;
        
        m_opaqueComponents.push_back(std::move(roof));
    }

    // Front grille
    {
        CarComponent grille;
        grille.mesh = ShapeBuilder::CreateBox();
        grille.shader = std::make_unique<Shader>();
        if (!grille.shader->CompileFromFiles("shaders/basic_texture.vert", "shaders/basic_texture.frag")) {
            grille.shader.reset();
        }
        
        grille.texture = Texture::CreateSolidColor(20, 20, 20, 255); // Black grille
        
        grille.position[0] = 0.0f;
        grille.position[1] = 0.5f;
        grille.position[2] = 2.3f; // Front of car
        grille.scale[0] = 1.2f;
        grille.scale[1] = 0.4f;
        grille.scale[2] = 0.05f;
        grille.isTransparent = false;
        
        m_opaqueComponents.push_back(std::move(grille));
    }

    // Headlights (left and right)
    {
        // Left headlight
        CarComponent headlightL;
        headlightL.mesh = ShapeBuilder::CreateBox();
        headlightL.shader = std::make_unique<Shader>();
        if (!headlightL.shader->CompileFromFiles("shaders/basic_texture.vert", "shaders/basic_texture.frag")) {
            headlightL.shader.reset();
        }
        
        headlightL.texture = Texture::CreateSolidColor(255, 255, 200, 255); // Yellow-white
        
        headlightL.position[0] = -0.7f;
        headlightL.position[1] = 0.4f;
        headlightL.position[2] = 2.25f;
        headlightL.scale[0] = 0.15f;
        headlightL.scale[1] = 0.2f;
        headlightL.scale[2] = 0.1f;
        headlightL.isTransparent = false;
        
        m_opaqueComponents.push_back(std::move(headlightL));

        // Right headlight
        CarComponent headlightR;
        headlightR.mesh = ShapeBuilder::CreateBox();
        headlightR.shader = std::make_unique<Shader>();
        if (!headlightR.shader->CompileFromFiles("shaders/basic_texture.vert", "shaders/basic_texture.frag")) {
            headlightR.shader.reset();
        }
        
        headlightR.texture = Texture::CreateSolidColor(255, 255, 200, 255);
        
        headlightR.position[0] = 0.7f;
        headlightR.position[1] = 0.4f;
        headlightR.position[2] = 2.25f;
        headlightR.scale[0] = 0.15f;
        headlightR.scale[1] = 0.2f;
        headlightR.scale[2] = 0.1f;
        headlightR.isTransparent = false;
        
        m_opaqueComponents.push_back(std::move(headlightR));
    }

    // Rear bumper
    {
        CarComponent bumper;
        bumper.mesh = ShapeBuilder::CreateBox();
        bumper.shader = std::make_unique<Shader>();
        if (!bumper.shader->CompileFromFiles("shaders/basic_texture.vert", "shaders/basic_texture.frag")) {
            bumper.shader.reset();
        }
        
        bumper.texture = Texture::CreateSolidColor(30, 50, 120, 255);
        
        bumper.position[0] = 0.0f;
        bumper.position[1] = 0.3f;
        bumper.position[2] = -2.3f; // Back of car
        bumper.scale[0] = 1.8f;
        bumper.scale[1] = 0.3f;
        bumper.scale[2] = 0.1f;
        bumper.isTransparent = false;
        
        m_opaqueComponents.push_back(std::move(bumper));
    }
}

void MainCar::CreateInterior() {
    // Floor/interior base
    {
        CarComponent floor;
        floor.mesh = ShapeBuilder::CreateBox();
        floor.shader = std::make_unique<Shader>();
        if (!floor.shader->CompileFromFiles("shaders/basic_texture.vert", "shaders/basic_texture.frag")) {
            floor.shader.reset();
        }
        
        floor.texture = Texture::CreateSolidColor(60, 40, 30, 255); // Dark brown floor
        
        floor.position[0] = 0.0f;
        floor.position[1] = 0.2f;
        floor.position[2] = 0.0f;
        floor.scale[0] = 1.4f;
        floor.scale[1] = 0.05f;
        floor.scale[2] = 3.5f;
        floor.isTransparent = false;
        
        m_opaqueComponents.push_back(std::move(floor));
    }

    // Dashboard (front interior panel) - more visible
    {
        CarComponent dashboard;
        dashboard.mesh = ShapeBuilder::CreateBox();
        dashboard.shader = std::make_unique<Shader>();
        if (!dashboard.shader->CompileFromFiles("shaders/basic_texture.vert", "shaders/basic_texture.frag")) {
            dashboard.shader.reset();
        }
        
        dashboard.texture = Texture::CreateSolidColor(100, 70, 50, 255); // Brown dashboard
        
        dashboard.position[0] = 0.0f;
        dashboard.position[1] = 0.65f;
        dashboard.position[2] = 1.8f; // Front of interior
        dashboard.scale[0] = 1.4f;
        dashboard.scale[1] = 0.4f;
        dashboard.scale[2] = 0.15f;
        dashboard.isTransparent = false;
        
        m_opaqueComponents.push_back(std::move(dashboard));
    }

    // Dashboard display/screen - larger and more visible
    {
        CarComponent display;
        display.mesh = ShapeBuilder::CreateQuad();
        display.shader = std::make_unique<Shader>();
        if (!display.shader->CompileFromFiles("shaders/basic_texture.vert", "shaders/basic_texture.frag")) {
            display.shader.reset();
        }
        
        display.texture = Texture::CreateSolidColor(0, 150, 255, 255); // Bright blue screen
        
        display.position[0] = 0.0f;
        display.position[1] = 0.7f;
        display.position[2] = 1.75f;
        display.scale[0] = 0.5f;
        display.scale[1] = 0.2f;
        display.scale[2] = 1.0f;
        display.rotation[1] = 90.0f; // Rotate to face forward
        display.isTransparent = false;
        
        m_opaqueComponents.push_back(std::move(display));
    }

    // Center console between seats
    {
        CarComponent console;
        console.mesh = ShapeBuilder::CreateBox();
        console.shader = std::make_unique<Shader>();
        if (!console.shader->CompileFromFiles("shaders/basic_texture.vert", "shaders/basic_texture.frag")) {
            console.shader.reset();
        }
        
        console.texture = Texture::CreateSolidColor(80, 60, 40, 255); // Brown console
        
        console.position[0] = 0.0f;
        console.position[1] = 0.35f;
        console.position[2] = 0.0f;
        console.scale[0] = 0.2f;
        console.scale[1] = 0.3f;
        console.scale[2] = 1.2f;
        console.isTransparent = false;
        
        m_opaqueComponents.push_back(std::move(console));
    }

    // Driver seat - more detailed
    {
        // Seat base
        CarComponent driverSeatBase;
        driverSeatBase.mesh = ShapeBuilder::CreateBox();
        driverSeatBase.shader = std::make_unique<Shader>();
        if (!driverSeatBase.shader->CompileFromFiles("shaders/basic_texture.vert", "shaders/basic_texture.frag")) {
            driverSeatBase.shader.reset();
        }
        
        driverSeatBase.texture = Texture::CreateSolidColor(120, 60, 60, 255); // Red seat
        
        driverSeatBase.position[0] = -0.4f; // Left side
        driverSeatBase.position[1] = 0.25f;  // Seat base height
        driverSeatBase.position[2] = 0.2f;  // Forward position
        driverSeatBase.scale[0] = 0.5f;
        driverSeatBase.scale[1] = 0.15f;
        driverSeatBase.scale[2] = 0.5f;
        driverSeatBase.isTransparent = false;
        
        m_opaqueComponents.push_back(std::move(driverSeatBase));

        // Seat back
        CarComponent driverSeatBack;
        driverSeatBack.mesh = ShapeBuilder::CreateBox();
        driverSeatBack.shader = std::make_unique<Shader>();
        if (!driverSeatBack.shader->CompileFromFiles("shaders/basic_texture.vert", "shaders/basic_texture.frag")) {
            driverSeatBack.shader.reset();
        }
        
        driverSeatBack.texture = Texture::CreateSolidColor(120, 60, 60, 255);
        
        driverSeatBack.position[0] = -0.4f;
        driverSeatBack.position[1] = 0.55f;
        driverSeatBack.position[2] = 0.0f;
        driverSeatBack.scale[0] = 0.5f;
        driverSeatBack.scale[1] = 0.5f;
        driverSeatBack.scale[2] = 0.15f;
        driverSeatBack.isTransparent = false;
        
        m_opaqueComponents.push_back(std::move(driverSeatBack));
        
        // Update driver seat position for camera (center of seat, eye level)
        m_driverSeatPos[0] = -0.4f;
        m_driverSeatPos[1] = 0.7f; // Eye level above seat
        m_driverSeatPos[2] = 0.2f;
        m_driverSeatYaw = 0.0f; // Facing forward
    }

    // Passenger seat - more detailed
    {
        // Seat base
        CarComponent passengerSeatBase;
        passengerSeatBase.mesh = ShapeBuilder::CreateBox();
        passengerSeatBase.shader = std::make_unique<Shader>();
        if (!passengerSeatBase.shader->CompileFromFiles("shaders/basic_texture.vert", "shaders/basic_texture.frag")) {
            passengerSeatBase.shader.reset();
        }
        
        passengerSeatBase.texture = Texture::CreateSolidColor(120, 60, 60, 255); // Red seat
        
        passengerSeatBase.position[0] = 0.4f;  // Right side
        passengerSeatBase.position[1] = 0.25f;
        passengerSeatBase.position[2] = 0.2f;
        passengerSeatBase.scale[0] = 0.5f;
        passengerSeatBase.scale[1] = 0.15f;
        passengerSeatBase.scale[2] = 0.5f;
        passengerSeatBase.isTransparent = false;
        
        m_opaqueComponents.push_back(std::move(passengerSeatBase));

        // Seat back
        CarComponent passengerSeatBack;
        passengerSeatBack.mesh = ShapeBuilder::CreateBox();
        passengerSeatBack.shader = std::make_unique<Shader>();
        if (!passengerSeatBack.shader->CompileFromFiles("shaders/basic_texture.vert", "shaders/basic_texture.frag")) {
            passengerSeatBack.shader.reset();
        }
        
        passengerSeatBack.texture = Texture::CreateSolidColor(120, 60, 60, 255);
        
        passengerSeatBack.position[0] = 0.4f;
        passengerSeatBack.position[1] = 0.55f;
        passengerSeatBack.position[2] = 0.0f;
        passengerSeatBack.scale[0] = 0.5f;
        passengerSeatBack.scale[1] = 0.5f;
        passengerSeatBack.scale[2] = 0.15f;
        passengerSeatBack.isTransparent = false;
        
        m_opaqueComponents.push_back(std::move(passengerSeatBack));
    }

    // Steering wheel - more visible
    {
        // Wheel rim
        CarComponent steeringWheel;
        steeringWheel.mesh = ShapeBuilder::CreateBox();
        steeringWheel.shader = std::make_unique<Shader>();
        if (!steeringWheel.shader->CompileFromFiles("shaders/basic_texture.vert", "shaders/basic_texture.frag")) {
            steeringWheel.shader.reset();
        }
        
        steeringWheel.texture = Texture::CreateSolidColor(30, 30, 30, 255); // Dark gray/black
        
        steeringWheel.position[0] = -0.4f; // Aligned with driver
        steeringWheel.position[1] = 0.65f; // Below dashboard
        steeringWheel.position[2] = 1.5f;  // Front of interior
        steeringWheel.scale[0] = 0.5f;  // Wheel diameter
        steeringWheel.scale[1] = 0.08f; // Wheel thickness
        steeringWheel.scale[2] = 0.5f;
        steeringWheel.rotation[0] = 90.0f; // Rotate to horizontal
        steeringWheel.isTransparent = false;
        
        m_opaqueComponents.push_back(std::move(steeringWheel));

        // Steering column
        CarComponent steeringColumn;
        steeringColumn.mesh = ShapeBuilder::CreateBox();
        steeringColumn.shader = std::make_unique<Shader>();
        if (!steeringColumn.shader->CompileFromFiles("shaders/basic_texture.vert", "shaders/basic_texture.frag")) {
            steeringColumn.shader.reset();
        }
        
        steeringColumn.texture = Texture::CreateSolidColor(40, 40, 40, 255);
        
        steeringColumn.position[0] = -0.4f;
        steeringColumn.position[1] = 0.5f;
        steeringColumn.position[2] = 1.5f;
        steeringColumn.scale[0] = 0.08f;
        steeringColumn.scale[1] = 0.3f;
        steeringColumn.scale[2] = 0.08f;
        steeringColumn.isTransparent = false;
        
        m_opaqueComponents.push_back(std::move(steeringColumn));
    }

    // Windshield (transparent) - larger and more visible
    {
        CarComponent windshield;
        windshield.mesh = ShapeBuilder::CreateQuad();
        windshield.shader = std::make_unique<Shader>();
        if (!windshield.shader->CompileFromFiles("shaders/transparent.vert", "shaders/transparent.frag")) {
            // Fallback to glass shader
            windshield.shader->CompileFromFiles("shaders/glass.vert", "shaders/glass.frag");
        }
        
        windshield.texture = Texture::CreateSolidColor(180, 200, 255, 150); // Light blue with transparency
        
        windshield.position[0] = 0.0f;
        windshield.position[1] = 1.0f;
        windshield.position[2] = 2.0f; // Front of car
        windshield.scale[0] = 1.4f;
        windshield.scale[1] = 0.6f;
        windshield.scale[2] = 1.0f;
        windshield.rotation[0] = -20.0f; // Angled windshield
        windshield.isTransparent = true;
        
        m_transparentComponents.push_back(std::move(windshield));
    }

    // Side windows (left and right) - larger for better interior visibility
    {
        // Left window
        CarComponent leftWindow;
        leftWindow.mesh = ShapeBuilder::CreateQuad();
        leftWindow.shader = std::make_unique<Shader>();
        if (!leftWindow.shader->CompileFromFiles("shaders/transparent.vert", "shaders/transparent.frag")) {
            leftWindow.shader->CompileFromFiles("shaders/glass.vert", "shaders/glass.frag");
        }
        
        leftWindow.texture = Texture::CreateSolidColor(180, 200, 255, 120); // More transparent
        
        leftWindow.position[0] = -0.9f; // Left side
        leftWindow.position[1] = 0.9f;
        leftWindow.position[2] = 0.0f;
        leftWindow.scale[0] = 1.0f;
        leftWindow.scale[1] = 0.7f;
        leftWindow.scale[2] = 1.0f;
        leftWindow.rotation[1] = 90.0f; // Face outward
        leftWindow.isTransparent = true;
        
        m_transparentComponents.push_back(std::move(leftWindow));

        // Right window
        CarComponent rightWindow;
        rightWindow.mesh = ShapeBuilder::CreateQuad();
        rightWindow.shader = std::make_unique<Shader>();
        if (!rightWindow.shader->CompileFromFiles("shaders/transparent.vert", "shaders/transparent.frag")) {
            rightWindow.shader->CompileFromFiles("shaders/glass.vert", "shaders/glass.frag");
        }
        
        rightWindow.texture = Texture::CreateSolidColor(180, 200, 255, 120);
        
        rightWindow.position[0] = 0.9f; // Right side
        rightWindow.position[1] = 0.9f;
        rightWindow.position[2] = 0.0f;
        rightWindow.scale[0] = 1.0f;
        rightWindow.scale[1] = 0.7f;
        rightWindow.scale[2] = 1.0f;
        rightWindow.rotation[1] = -90.0f; // Face outward
        rightWindow.isTransparent = true;
        
        m_transparentComponents.push_back(std::move(rightWindow));
    }

    // Rear window
    {
        CarComponent rearWindow;
        rearWindow.mesh = ShapeBuilder::CreateQuad();
        rearWindow.shader = std::make_unique<Shader>();
        if (!rearWindow.shader->CompileFromFiles("shaders/transparent.vert", "shaders/transparent.frag")) {
            rearWindow.shader->CompileFromFiles("shaders/glass.vert", "shaders/glass.frag");
        }
        
        rearWindow.texture = Texture::CreateSolidColor(180, 200, 255, 120);
        
        rearWindow.position[0] = 0.0f;
        rearWindow.position[1] = 1.0f;
        rearWindow.position[2] = -1.8f; // Back of car
        rearWindow.scale[0] = 1.2f;
        rearWindow.scale[1] = 0.5f;
        rearWindow.scale[2] = 1.0f;
        rearWindow.rotation[0] = 20.0f; // Angled
        rearWindow.isTransparent = true;
        
        m_transparentComponents.push_back(std::move(rearWindow));
    }
}

void MainCar::CreateWheels() {
    // Create 4 wheels - make them more visible and car-like
    float wheelRadius = 0.35f;
    float wheelWidth = 0.25f;
    float wheelY = 0.15f; // Wheel center height (lower for better visibility)
    
    // Wheel positions relative to car center
    float positions[4][3] = {
        {-0.85f, wheelY,  1.5f},  // Front-left
        { 0.85f, wheelY,  1.5f},  // Front-right
        {-0.85f, wheelY, -1.5f},  // Back-left
        { 0.85f, wheelY, -1.5f}   // Back-right
    };

    for (int i = 0; i < 4; i++) {
        // Wheel rim (outer part)
        CarComponent wheel;
        wheel.mesh = ShapeBuilder::CreateBox();
        wheel.shader = std::make_unique<Shader>();
        if (!wheel.shader->CompileFromFiles("shaders/basic_texture.vert", "shaders/basic_texture.frag")) {
            wheel.shader.reset();
        }
        
        wheel.texture = Texture::CreateSolidColor(40, 40, 40, 255); // Dark gray/black wheels
        
        wheel.position[0] = positions[i][0];
        wheel.position[1] = positions[i][1];
        wheel.position[2] = positions[i][2];
        wheel.scale[0] = wheelWidth;
        wheel.scale[1] = wheelRadius * 2.0f;
        wheel.scale[2] = wheelRadius * 2.0f;
        wheel.rotation[0] = 0.0f;
        wheel.rotation[1] = 90.0f; // Rotate to face forward
        wheel.rotation[2] = 0.0f;
        wheel.isTransparent = false;
        
        m_opaqueComponents.push_back(std::move(wheel));

        // Wheel hub (center part)
        CarComponent hub;
        hub.mesh = ShapeBuilder::CreateBox();
        hub.shader = std::make_unique<Shader>();
        if (!hub.shader->CompileFromFiles("shaders/basic_texture.vert", "shaders/basic_texture.frag")) {
            hub.shader.reset();
        }
        
        hub.texture = Texture::CreateSolidColor(100, 100, 100, 255); // Silver hub
        
        hub.position[0] = positions[i][0];
        hub.position[1] = positions[i][1];
        hub.position[2] = positions[i][2];
        hub.scale[0] = wheelWidth * 0.6f;
        hub.scale[1] = wheelRadius * 1.2f;
        hub.scale[2] = wheelRadius * 1.2f;
        hub.rotation[0] = 0.0f;
        hub.rotation[1] = 90.0f;
        hub.isTransparent = false;
        
        m_opaqueComponents.push_back(std::move(hub));
    }
}

void MainCar::BuildModelMatrix(const CarComponent& comp, float out[16]) const {
    // Build transformation matrix: Translate -> Rotate -> Scale
    // Column-major format for OpenGL
    
    // Start with identity
    for (int i = 0; i < 16; i++) out[i] = 0.0f;
    out[0] = out[5] = out[10] = out[15] = 1.0f;
    
    // Apply car's world transform first
    float carX = m_position[0];
    float carY = m_position[1];
    float carZ = m_position[2];
    float carRot = m_rotation * 3.14159f / 180.0f; // Convert to radians
    
    // Apply component's local transform relative to car
    float px = comp.position[0];
    float py = comp.position[1];
    float pz = comp.position[2];
    
    // Rotate component position by car rotation
    float cosR = std::cos(carRot);
    float sinR = std::sin(carRot);
    float rotX = px * cosR - pz * sinR;
    float rotZ = px * sinR + pz * cosR;
    
    // Final position
    float fx = carX + rotX;
    float fy = carY + py;
    float fz = carZ + rotZ;
    
    // Apply component rotation (simplified - just Y rotation for most parts)
    float compRotY = comp.rotation[1] * 3.14159f / 180.0f;
    float compRotX = comp.rotation[0] * 3.14159f / 180.0f;
    
    // Build rotation matrix (combined car + component rotation)
    float totalRotY = carRot + compRotY;
    float cosRY = std::cos(totalRotY);
    float sinRY = std::sin(totalRotY);
    float cosRX = std::cos(compRotX);
    float sinRX = std::sin(compRotX);
    
    // Scale
    float sx = comp.scale[0];
    float sy = comp.scale[1];
    float sz = comp.scale[2];
    
    // Build final matrix: M = T * R * S (column-major)
    out[0] = cosRY * sx;
    out[1] = sinRX * sinRY * sy;
    out[2] = -cosRX * sinRY * sz;
    out[4] = 0.0f;
    out[5] = cosRX * sy;
    out[6] = sinRX * sz;
    out[8] = sinRY * sx;
    out[9] = -sinRX * cosRY * sy;
    out[10] = cosRX * cosRY * sz;
    out[12] = fx;
    out[13] = fy;
    out[14] = fz;
    out[15] = 1.0f;
}

void MainCar::RenderComponent(const CarComponent& comp, const Camera& camera) const {
    if (!comp.mesh || !comp.shader) return;
    
    comp.shader->Use();
    
    // Build and set model matrix
    float model[16];
    BuildModelMatrix(comp, model);
    comp.shader->SetUniformMat4("uModel", model);
    
    // Apply camera matrices (use Renderer's stored camera or fallback)
    if (Renderer::HasCamera()) {
        Renderer::ApplyCameraToShader(*comp.shader);
    } else {
        // Fallback to provided camera
        comp.shader->SetUniformMat4("uView", camera.view);
        comp.shader->SetUniformMat4("uProj", camera.projection);
    }
    
    // Bind texture if available
    if (comp.texture) {
        comp.shader->SetUniformInt("uTexture", 0);
        comp.texture->Bind(0);
    }
    
    // Render
    Renderer::EnableDepth(true);
    Renderer::EnableBlending(comp.isTransparent);
    Renderer::DrawMesh(*comp.mesh, *comp.shader);
}

void MainCar::RenderOpaque() {
    // Get camera from scene (we'll pass it through CarBase later, for now use default)
    Camera defaultCamera;
    // TODO: Get actual camera from scene manager
    
    for (const auto& comp : m_opaqueComponents) {
        RenderComponent(comp, defaultCamera);
    }
}

void MainCar::RenderTransparent() {
    Camera defaultCamera;
    // TODO: Get actual camera from scene manager
    
    for (const auto& comp : m_transparentComponents) {
        RenderComponent(comp, defaultCamera);
    }
}

void MainCar::Destroy() {
    // Clean up textures
    for (auto& comp : m_opaqueComponents) {
        if (comp.texture && comp.texture != Texture::CreateSolidColor(0,0,0,255)) {
            comp.texture->Destroy();
            delete comp.texture;
            comp.texture = nullptr;
        }
    }
    for (auto& comp : m_transparentComponents) {
        if (comp.texture && comp.texture != Texture::CreateSolidColor(0,0,0,255)) {
            comp.texture->Destroy();
            delete comp.texture;
            comp.texture = nullptr;
        }
    }
    
    m_opaqueComponents.clear();
    m_transparentComponents.clear();
}

void MainCar::GetDriverSeatPosition(float& x, float& y, float& z) const {
    // Return world position of driver seat
    float carRot = m_rotation * 3.14159f / 180.0f;
    float cosR = std::cos(carRot);
    float sinR = std::sin(carRot);
    
    // Rotate driver seat position by car rotation
    float rotX = m_driverSeatPos[0] * cosR - m_driverSeatPos[2] * sinR;
    float rotZ = m_driverSeatPos[0] * sinR + m_driverSeatPos[2] * cosR;
    
    x = m_position[0] + rotX;
    y = m_position[1] + m_driverSeatPos[1];
    z = m_position[2] + rotZ;
}

void MainCar::GetDriverSeatRotation(float& yaw) const {
    yaw = m_rotation + m_driverSeatYaw;
}
