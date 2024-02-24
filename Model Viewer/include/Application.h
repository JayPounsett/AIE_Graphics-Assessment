#pragma once

#include "GLFW/glfw3.h"
#include "Mesh.h"
#include "Shader.h"
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
    bool startup();
    bool update();
    void draw();
    void shutdown();

protected:
    // Window Variables
    GLFWwindow* m_window;
    int m_windowWidth = 1280;
    int m_windowHeight = 720;

    // Colour Variables
    glm::vec4 WHITE { 1, 1, 1, 1 };
    glm::vec4 BLACK { 0, 0, 0, 1 };

    // Meshes & Transforms
protected:
    aie::ShaderProgram m_shader;
    Mesh m_quadMesh;
    glm::mat4 m_quadTransform;

    // Grid & Viewport
private:
    void createGrid() const;
    void createViewPort() const;

    // Projection & View
    // glm::vec3 eyePosition = glm::vec3{10, 10, 10};
    // glm::vec3 lookAtPosition = glm::vec3(1);
    // glm::vec3 upDirection = glm::vec3(0, 1, 0);

    // float fov90 = glm::pi<float>() * 0.25f;
    // float aspect16x9 = 16 / 9.f;
    // float nearClip = 0.1f;
    // float farClip = 1000.f;

    glm::mat4 m_view = glm::lookAt( glm::vec3( 10, 10, 10 ), glm::vec3( 1 ), glm::vec3( 0, 1, 0 ) );
    glm::mat4 m_projection = glm::perspective( glm::pi<float>() * 0.25f, 16 / 9.f, 0.1f, 1000.f );
};
