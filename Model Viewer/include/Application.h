#pragma once

#include "GLFW/glfw3.h"
#include "Camera.h"
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

class Application {
public:
  bool Startup();
  bool Update();
  void Draw();
  void Shutdown();

  // Singleton Pattern
  static Application *Get() { return p_instance; }

  glm::vec2 getMousePosition() { return m_mousePosition; }
  glm::vec2 GetMouseDelta() { return m_mousePosition - m_lastMousePosition; }

  // Set up mouse input
  static void SetMousePosition(GLFWwindow *window, double x, double y);

protected:
  static Application *p_instance;

  // Window Variables
  GLFWwindow *p_window;
  int m_windowWidth = 1280;
  int m_windowHeight = 720;

  // Colour Variables
  glm::vec4 WHITE{1, 1, 1, 1};
  glm::vec4 BLACK{0, 0, 0, 1};

  // Meshes & Transforms
  aie::ShaderProgram m_shaderSimple;
  aie::ShaderProgram m_shaderPhong;

  Mesh m_quadMesh;
  glm::mat4 m_quadTransform;

  Mesh m_bunnyMesh;
  glm::mat4 m_bunnyTransform;

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

  // Camera
  Camera m_camera;
  glm::vec2 m_mousePosition;
  glm::vec2 m_lastMousePosition;

  // Lighting
  struct Light {
    glm::vec3 direction;
    glm::vec3 colour;
  };

  Light m_light;
  glm::vec3 m_ambientLight;

private:
  void CreateGrid() const;
};
