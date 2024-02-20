#pragma once

#include "GLFW/glfw3.h"
#include "Gizmos.h"
#include "glad.h"
#include "glm/ext.hpp"
#include "glm/glm.hpp"
#include "mesh_renderer.h"

using namespace aie;

class Application {
 public:
  bool Startup();
  bool Update();
  void Draw();
  void Shutdown();

 private:
  void CreateGrid(const int number_of_grid_lines,
                  const glm::vec4 middle_line_colour,
                  const glm::vec4 grid_line_colour);

  void CreateViewPort(const glm::mat4 projection, const glm::mat4 view) {
    Gizmos::draw(projection * view);
  };

 protected:
  // Colour Variables
  glm::vec4 kWHITE{1, 1, 1, 1};
  glm::vec4 kBLACK{0, 0, 0, 1};

  // Meshes
  MeshRenderer mesh_render;

 private:
  // Window Variables
  GLFWwindow *window;
  int window_width = 1280;
  int window_height = 720;

  // View & Perspective Variables
  glm::vec3 eye_position = glm::vec3{10, 10, 10};
  glm::vec3 look_at_position = glm::vec3(1);
  glm::vec3 up_direction = glm::vec3(0, 1, 0);

  float fov_90 = glm::pi<float>() * 0.25f;
  float aspect_16x9 = 16 / 9.f;
  float near_clip_plane = 0.1f;
  float far_clip_plane = 1000.f;

  glm::mat4 view = glm::lookAt(eye_position, look_at_position, up_direction);
  glm::mat4 projection =
      glm::perspective(fov_90, aspect_16x9, near_clip_plane, far_clip_plane);

  // Grid Variables
  int number_of_grid_lines = 21;
};
