#pragma once

#include "glad.h"
#include "MeshRenderer.h"

#include "GLFW/glfw3.h"
#include "Gizmos.h"
#include "glm/ext.hpp"
#include "glm/glm.hpp"

using namespace aie;

class Application {
public:
  bool Startup();
  bool Update();
  void Draw();
  void Shutdown();

private:
  void CreateGrid(const int numGridLines, const glm::vec4 midLineColour,
				  const glm::vec4 gridColour);

  void CreateViewPort(const glm::mat4 projection, const glm::mat4 view) {
	Gizmos::draw(projection * view);
  };

protected:
  // Window Variables
  GLFWwindow *p_Window;
  const int k_WindowWidth = 1280;
  const int k_WindowHeight = 720;

  // View & Perspective Variables
  const glm::vec3 k_EyePos = glm::vec3{10, 10, 10};
  const glm::vec3 k_LookAtPos = glm::vec3(1);
  const glm::vec3 k_UpDirection = glm::vec3(0, 1, 0);

  const float k_Fov90 = glm::pi<float>() * 0.25f;
  const float k_Aspect16x9 = 16 / 9.f;
  const float k_NearClipPlane = 0.1f;
  const float k_FarClipPlane = 1000.f;

  const glm::mat4 k_View = glm::lookAt(k_EyePos, k_LookAtPos, k_UpDirection);
  const glm::mat4 k_Projection =
	  glm::perspective(k_Fov90, k_Aspect16x9, k_NearClipPlane, k_FarClipPlane);

  // Grid Variables
  const int k_NumGridLines = 21;

  // Colour Variables
  const glm::vec4 k_White{1, 1, 1, 1};
  const glm::vec4 k_Black{0, 0, 0, 1};

  // Meshes
  MeshRenderer c_MeshRender;
};
