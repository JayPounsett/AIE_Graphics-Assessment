#pragma once

#include "Camera.h"
#include "GLFW/glfw3.h"
#include "Mesh.h"
#include "Scene.h"
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

class Instance;

class Application {
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
    Scene* activeScene;

    // Window Variables
    GLFWwindow* m_window;
    const int WINDOW_WIDTH = 1600;
    const int WINDOW_HEIGHT = 900;

    Camera m_camera;

    // Colour Variables
    glm::vec4 WHITE { 1, 1, 1, 1 };
    glm::vec4 BLACK { 0, 0, 0, 1 };

    // Shaders
    aie::ShaderProgram m_simpleShader;
    aie::ShaderProgram m_simplePhongShader;
    aie::ShaderProgram m_normalPhongShader;

    // Meshes
    Mesh m_quadMesh;
    Mesh m_bunnyMesh;
    Mesh m_soulspearMesh;

    // Textures
    aie::Texture m_gridTexture;

    // Projection & View
    glm::mat4 m_projection;
    glm::mat4 m_view;

    glm::vec3 m_ambientLight;

private:
    void CreateGrid() const;

    glm::vec2 m_mousePosition;
    glm::vec2 m_lastMousePosition;
};
