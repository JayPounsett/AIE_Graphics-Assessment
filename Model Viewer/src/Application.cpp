#include "application.h"

#include <iostream>

using namespace aie;

bool Application::Startup() {
  if (glfwInit() == false) {
	return false;
  }

  window = glfwCreateWindow(window_width, window_height, "Model Viewer",
							 nullptr, nullptr);

  if (window == nullptr) {
	glfwTerminate();
	return false;
  }

  glfwMakeContextCurrent(window);

  if (!gladLoadGL()) {
	glfwDestroyWindow(window);
	glfwTerminate();
	return false;
  }

  printf("GL: %i.%i\n", GLVersion.major, GLVersion.minor);

  glClearColor(0.25f, 0.25f, 0.25f, 1); // Grey
  glEnable(GL_DEPTH_TEST);

  Gizmos::create(10000, 10000, 0, 0);

  return true;
}

bool Application::Update() {
  if (glfwWindowShouldClose(window) == true ||
	  glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
	return false;
  }
  return true;
}

void Application::Draw() {
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  Gizmos::clear();

  Gizmos::addTransform(glm::mat4(1)); // 3-Colour XYZ Tool

  CreateGrid(number_of_grid_lines, kWHITE, kBLACK); // 10x10 square grid (21 x 21 grid lines)
  CreateViewPort(projection, view);

  //////////// Everything goes in here related to drawing ////////////

  glfwSwapBuffers(window);
  glfwPollEvents();
}

void Application::Shutdown() {
  glfwDestroyWindow(window);
  glfwTerminate();
}

void Application::CreateGrid(const int number_of_grid_lines,
							 const glm::vec4 middle_line_colour,
							 const glm::vec4 grid_line_colour) {

  int mid_point = (number_of_grid_lines - 1) / 2;

  for (int i = 0; i < number_of_grid_lines; i++) {
	Gizmos::addLine(glm::vec3(-mid_point + i, 0, mid_point),
					glm::vec3(-mid_point + i, 0, -mid_point),
					i == mid_point ? middle_line_colour : grid_line_colour);
	Gizmos::addLine(glm::vec3(mid_point, 0, -mid_point + i),
					glm::vec3(-10, 0, -10 + i),
					i == mid_point ? middle_line_colour : grid_line_colour);
  }
}
