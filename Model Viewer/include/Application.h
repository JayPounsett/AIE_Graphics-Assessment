#pragma once

#include "Camera.h"
#include "GLFW/glfw3.h"
#include "Mesh.h"
#include "Scene.h"
#include "Shader.h"
#include "Skybox.h"
#include "Texture.h"
#include "glad.h"
#include "glm/ext.hpp"
#include "glm/glm.hpp"

class Instance;

class Application {
public:
  bool Startup();
  bool Update();
  void Draw();
  void Shutdown();

  // Singleton Pattern
  static Application* get() { return instance; }

  glm::vec2 GetMousePosition() { return mousePosition; }
  glm::vec2 GetMouseDelta() { return mousePosition - lastMousePosition; }

  // Set up mouse input
  static void SetMousePosition(GLFWwindow* window, double x, double y);

protected:
  static Application* instance;
  Scene* activeScene;

  // Window Variables
  GLFWwindow* window;
  const int kWindowWidth = 1600;
  const int kWindowHeight = 900;

  // Camera & Lighting
  Camera camera;
  glm::vec3 ambientLight;

  // Colour Variables
  glm::vec4 kWhite{1, 1, 1, 1};
  glm::vec4 kBlack{0, 0, 0, 1};

  // Instances
  Instance* quadInstance;
  Instance* bunnyInstance;
  Instance* soulspearInstance;

  // Shaders
  aie::ShaderProgram simpleShader;
  aie::ShaderProgram simplePhongShader;
  aie::ShaderProgram phongNoTextureShader;
  aie::ShaderProgram normalPhongShader;
  aie::ShaderProgram skyboxShader;
  aie::ShaderProgram reflectionShader;

  // Meshes
  Skybox skybox;
  Mesh quadMesh;
  Mesh bunnyMesh;
  Mesh soulspearMesh;
  Mesh dragonMesh;
  Mesh mirrorCube;

  glm::mat4 mirrorCubeTransform;

  unsigned int cubemapTexture;

  // Projection & View
  glm::mat4 projection;
  glm::mat4 view;

private:
  void CreateGrid() const;

  glm::vec2 mousePosition;
  glm::vec2 lastMousePosition;
};
