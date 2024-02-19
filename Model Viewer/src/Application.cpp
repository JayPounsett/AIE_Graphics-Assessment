#include "Application.h"

#include <iostream>

using namespace aie;

bool Application::Startup() {
  if (glfwInit() == false) {
	return false;
  }

  p_Window = glfwCreateWindow(k_WindowWidth, k_WindowHeight, "Model Viewer",
							 nullptr, nullptr);

  if (p_Window == nullptr) {
	glfwTerminate();
	return false;
  }

  glfwMakeContextCurrent(p_Window);

  if (!gladLoadGL()) {
	glfwDestroyWindow(p_Window);
	glfwTerminate();
	return false;
  }

  printf("GL: %i.%i\n", GLVersion.major, GLVersion.minor);

  glClearColor(0.25f, 0.25f, 0.25f, 1); // Grey
  glEnable(GL_DEPTH_TEST);

  Gizmos::create(10000, 10000, 0, 0);

  //m_meshRender.InitialiseQuad();

  return true;
}

bool Application::Update() {
  if (glfwWindowShouldClose(p_Window) == true ||
	  glfwGetKey(p_Window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
	return false;
  }
  return true;
}

void Application::Draw() {
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  Gizmos::clear();

  Gizmos::addTransform(glm::mat4(1)); // 3-Colour XYZ Tool

  CreateGrid(k_NumGridLines, k_White, k_Black); // 10x10 square grid (21 x 21 grid lines)
  CreateViewPort(k_Projection, k_View);

  //////////// Everything goes in here related to drawing ////////////

  glfwSwapBuffers(p_Window);
  glfwPollEvents();
}

void Application::Shutdown() {
  glfwDestroyWindow(p_Window);
  glfwTerminate();
}

void Application::CreateGrid(const int kNumGridLines,
							 const glm::vec4 midLineColour,
							 const glm::vec4 gridColour) {

  int midPoint = (kNumGridLines - 1) / 2;

  for (int i = 0; i < kNumGridLines; i++) {
	Gizmos::addLine(glm::vec3(-midPoint + i, 0, midPoint),
					glm::vec3(-midPoint + i, 0, -midPoint),
					i == midPoint ? midLineColour : gridColour);
	Gizmos::addLine(glm::vec3(midPoint, 0, -midPoint + i),
					glm::vec3(-10, 0, -10 + i),
					i == midPoint ? midLineColour : gridColour);
  }
}
