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
  static Application *get() { return instance; }

  glm::vec2 getMousePosition() { return mousePosition; }
  glm::vec2 getMouseDelta() { return mousePosition - lastMousePosition; }

  // Set up mouse input
  static void SetMousePosition(GLFWwindow *window, double x, double y);

protected:
  static Application *instance;

  // Window Variables
  GLFWwindow *window;
  int windowWidth = 1280;
  int windowHeight = 720;

  // Colour Variables
  glm::vec4 WHITE{1, 1, 1, 1};
  glm::vec4 BLACK{0, 0, 0, 1};

  // Meshes & Transforms
  aie::ShaderProgram shader;

  Mesh quadMesh;
  glm::mat4 quadTransform;

  Mesh bunnyMesh;
  glm::mat4 bunnyTransform;

  // Projection & View
  glm::mat4 projection;
  glm::mat4 view;

  // glm::vec3 eyePosition = glm::vec3{10, 10, 10};
  // glm::vec3 lookAtPosition = glm::vec3(1);
  // glm::vec3 upDirection = glm::vec3(0, 1, 0);

  // float fov90 = glm::pi<float>() * 0.25f;
  // float aspect16x9 = 16 / 9.f;
  // float nearClip = 0.1f;
  // float farClip = 1000.f;

  // Camera
  Camera camera;
  glm::vec2 mousePosition;
  glm::vec2 lastMousePosition;

  // Lighting
  struct Light {
    glm::vec3 direction;
    glm::vec3 colour;
  };

  Light light;
  glm::vec3 ambientLight;

private:
  void CreateGrid() const;
};
