#pragma once

#include "Camera.h"
#include "GLFW/glfw3.h"
#include "Mesh.h"
#include "Shader.h"
#include "Texture.h"
#include "glad.h"
#include "glm/ext.hpp"
#include "glm/glm.hpp"

// -= Future Refactoring =-
// Entity Component System?
// Application class: window, view, perspective, aspect ratio, FOV, clip planes,
// plus virtual functions Object class: transform, position, etc Mesh Class:
// mesh data, etc MeshRenderer Class: shaders, colours, Draw(), LoadMaterial(),
// ApplyMaterial() etc
// ... and so on

class Application
{
public:
    bool Startup();
    bool Update();
    void Draw();
    void Shutdown();

    // Singleton Pattern
    static Application* Get() { return m_instance; }

    glm::vec2 GetMousePosition() { return m_mousePosition; }
    glm::vec2 GetMouseDelta() { return m_mousePosition - m_lastMousePosition; }

    // Set up mouse input
    static void SetMousePosition(GLFWwindow* window, double x, double y);

protected:
    static Application* m_instance;

    // Window Variables
    GLFWwindow* m_window;
    int m_windowWidth = 1600;
    int m_windowHeight = 900;

    Camera m_camera;

    // Colour Variables
    glm::vec4 WHITE { 1, 1, 1, 1 };
    glm::vec4 BLACK { 0, 0, 0, 1 };

    // Shaders
    aie::ShaderProgram m_shader;
    aie::ShaderProgram m_phongShader;
    aie::ShaderProgram m_normalMapShader;

    // Meshes
    Mesh m_quadMesh;
    Mesh m_bunnyMesh;
    Mesh m_soulspearMesh;

    // Textures
    aie::Texture m_gridTexture;

    // Transforms
    glm::mat4 m_quadTransform;
    glm::mat4 m_bunnyTransform;
    glm::mat4 m_soulspearTransform;

    // Projection & View
    glm::mat4 m_projection;
    glm::mat4 m_view;

    // glm::vec3 eyePosition = glm::vec3{10, 10, 10};
    // glm::vec3 lookAtPosition = glm::vec3(1);
    // glm::vec3 upDirection = glm::vec3(0, 1, 0);

    // float fov90 = glm::pi<float>() * 0.25f;
    // float aspect16x9 = 16 / 9.f;
    // float nearClip = 0.1f;
    // float farClip = 1000.f;

    // Lighting
    struct Light
    {
        glm::vec3 direction;
        glm::vec3 colour;
    };

    Light m_light;
    glm::vec3 m_ambientLight;

private:
    void CreateGrid() const;

    glm::vec2 m_mousePosition;
    glm::vec2 m_lastMousePosition;
};
