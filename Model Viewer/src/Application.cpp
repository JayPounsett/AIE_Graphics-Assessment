#include "Application.h"
#include "Camera.h"
#include "Gizmos.h"
#include "imgui_glfw3.h"
#include <iostream>

Application *Application::instance;

bool Application::Startup() {
  if (glfwInit() == false) {
    return false;
  }

  // Create Window
  window = glfwCreateWindow(windowWidth, windowHeight, "OpenGL Class Project",
                            nullptr, nullptr);

  if (window == nullptr) {
    glfwTerminate();
    return false;
  }

  glfwMakeContextCurrent(
      window); // Sets specified window as the one that will be used

  if (!gladLoadGL()) {
    glfwDestroyWindow(window);
    glfwTerminate();
    return false;
  }

  printf("GL: %i.%i\n", GLVersion.major, GLVersion.minor);

  glClearColor(0.25, 0.25, 0.25, 1); // Grey
  glEnable(GL_DEPTH_TEST);

  aie::Gizmos::create(10000, 10000, 0, 0);
  aie::ImGui_Init(window, true);

  // Setting Up Mouse Input
  instance = this;

  glfwSetCursorPosCallback(window, &Application::SetMousePosition);

  // Creating viewport
  view = glm::lookAt(glm::vec3(10, 10, 10), glm::vec3(0), glm::vec3(0, 1, 0));
  projection =
      glm::perspective(glm::pi<float>() * 0.25f, 16 / 9.f, 0.1f, 1000.f);

  // Load shaders
  shader.loadShader(aie::eShaderStage::VERTEX, "./Shader/Vertex/phong.vert");
  shader.loadShader(aie::eShaderStage::FRAGMENT,
                    "./Shader/Fragment/phong.frag");

  if (shader.link() == false) {
    printf("Shader Error: %s\n", shader.getLastError());
    return false;
  }

  quadMesh.InitialiseQuad();

  // quad transform - Make it 10 units wide and high
  quadTransform = {10, 0, 0, 0, 0, 10, 0, 0, 0, 0, 10, 0, 0, 0, 0, 1};

  // Setting Light & Ambient Colours
  light.direction = glm::normalize(glm::vec3(-1));
  light.colour = {1, 1, 0};
  ambientLight = {0.25f, 0.25f, 0.25f};

  // Load Model from File
  bunnyMesh.InitialiseFromFile("./Model/Bunny.obj");
  bunnyMesh.LoadMaterial("./Model/Bunny.mtl");

  // Halving the size of the bunny so it fits better
  bunnyTransform = {0.5, 0, 0, 0, 0, 0.5, 0, 0, 0, 0, 0.5, 0, 0, 0, 0, 1};

  return true;
}

// Set Mouse Position Function
void Application::SetMousePosition(GLFWwindow *window, double x, double y) {
  instance->mousePosition.x = (float)x;
  instance->mousePosition.y = (float)y;
}

bool Application::Update() {
  aie::ImGui_NewFrame();
  ImGui::Begin("Light Settings");
  ImGui::DragFloat3("Sunlight Direction", &light.direction[0], 0.1f, -1.0f,
                    1.0f);
  ImGui::DragFloat3("Sunlight Colour", &light.colour[0], 0.1f, 0.0f, 2.0f);
  ImGui::End();

  camera.Update(0.1f, window);
  lastMousePosition = mousePosition;

  // Query time since application started
  float time = glfwGetTime();

  // Close window on ESC or pressing X in top right
  if (glfwWindowShouldClose(window) == true ||
      glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
    return false;
  }

  return true;
}

void Application::Draw() {
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  glm::mat4 pv = camera.GetProjectionMatrix(windowWidth, windowHeight) *
                 camera.GetViewMatrix();

  aie::Gizmos::clear();

  aie::Gizmos::addTransform(glm::mat4(1));

  ///// Everything goes below here related to drawing /////

  // Create 10x10 grid
  CreateGrid();

  aie::Gizmos::draw(pv);

  // Bind shader
  shader.bind();

  // Bind lighting
  shader.bindUniform("AmbientColour", ambientLight);
  shader.bindUniform("LightColour", light.colour);
  shader.bindUniform("LightDirection", light.direction);

  // Bind camera position
  shader.bindUniform("CameraPosition", camera.GetPosition());

  // Quad
  auto pvm = pv * quadTransform;
  shader.bindUniform("ProjectionViewModel", pvm);
  shader.bindUniform("ModelMatrix", quadTransform);
  quadMesh.ApplyMaterial(&shader);
  quadMesh.Draw();

  // Bunny
  pvm = pv * bunnyTransform;
  shader.bindUniform("ProjectionViewModel", pvm);
  shader.bindUniform("ModelMatrix", bunnyTransform);
  bunnyMesh.ApplyMaterial(&shader);
  bunnyMesh.Draw();

  ImGui::Render();

  glfwSwapBuffers(window); // Draw the front, then the back, and alternate.
  glfwPollEvents();        // Windows related stuff - moving the window, etc.
}

void Application::Shutdown() {
  aie::ImGui_Shutdown();
  aie::Gizmos::destroy();
  glfwDestroyWindow(window);
  glfwTerminate();
}

/// <summary>
/// Creates a 10x10 grid of black lines with the centre
/// crossed lines being white.
/// </summary>
void Application::CreateGrid() const {
  for (int i = 0; i < 21; i++) {
    aie::Gizmos::addLine(glm::vec3(-10 + i, 0, 10), glm::vec3(-10 + i, 0, -10),
                         i == 10 ? WHITE : BLACK);
    aie::Gizmos::addLine(glm::vec3(10, 0, -10 + i), glm::vec3(-10, 0, -10 + i),
                         i == 10 ? WHITE : BLACK);
  }
}
