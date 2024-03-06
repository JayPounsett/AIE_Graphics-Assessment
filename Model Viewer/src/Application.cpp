#include "Application.h"

#include <iostream>

#include "Camera.h"
#include "Gizmos.h"
#include "Instance.h"
#include "Scene.h"
#include "imgui_glfw3.h"

Application* Application::instance;

bool Application::Startup()
{
  if (glfwInit() == false) {
    return false;
  }

  // Create Window
  window = glfwCreateWindow(
    kWindowWidth, kWindowHeight, "OpenGL Class Project", nullptr, nullptr);

  if (window == nullptr) {
    glfwTerminate();
    return false;
  }

  // Sets specified window as the one that will be used
  glfwMakeContextCurrent(window);

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
  // Shader List: simpleShader, phongShader, phongNoTexture, normalPhongShader
  simpleShader.loadShader(
    aie::eShaderStage::VERTEX, "./Shaders/Vertex/simpleShader.vert");
  simpleShader.loadShader(
    aie::eShaderStage::FRAGMENT, "./Shaders/Fragment/simpleShader.frag");

  simplePhongShader.loadShader(
    aie::eShaderStage::VERTEX, "./Shaders/Vertex/phongShader.vert");
  simplePhongShader.loadShader(
    aie::eShaderStage::FRAGMENT, "./Shaders/Fragment/phongShader.frag");

  phongNoTextureShader.loadShader(
    aie::eShaderStage::VERTEX, "./Shaders/Vertex/phongNoTextureShader.vert");
  phongNoTextureShader.loadShader(
    aie::eShaderStage::FRAGMENT,
    "./Shaders/Fragment/phongNoTextureShader.frag");

  normalPhongShader.loadShader(
    aie::eShaderStage::VERTEX, "./Shaders/Vertex/normalPhongShader.vert");
  normalPhongShader.loadShader(
    aie::eShaderStage::FRAGMENT, "./Shaders/Fragment/normalPhongShader.frag");

  if (simpleShader.link() == false) {
    printf("Shader Error: %s\n", simpleShader.getLastError());
    return false;
  }

  if (simplePhongShader.link() == false) {
    printf("Shader Error: %s\n", simplePhongShader.getLastError());
    return false;
  }

  if (phongNoTextureShader.link() == false) {
    printf("Shader Error: %s\n", phongNoTextureShader.getLastError());
    return false;
  }

  if (normalPhongShader.link() == false) {
    printf("Shader Error: %s\n", normalPhongShader.getLastError());
    return false;
  }

  quadMesh.InitialiseQuad();
  quadMesh.CreateMaterial(
    glm::vec3(1), glm::vec3(1), glm::vec3(1), "./Textures/numbered_grid.tga");

  glm::mat4 quadTransform = {10, 0, 0, 0, 0, 10, 0, 0, 0, 0, 10, 0, 0, 0, 0, 1};
  glm::mat4 bunnyTransform = {
    0.5, 0, 0, 0, 0, 0.5, 0, 0, 0, 0, 0.5, 0, 3, 0, -3, 1};

  glm::mat4 lucyTransform = {
    0.5, 0, 0, 0, 0, 0.5, 0, 0, 0, 0, 0.5, 0, 0, 0, 0, 1};

  // 4th last digit will move on X-axis, lesson used 5 to move the spear beside
  // the bunny
  glm::mat4 soulspearTransform = {
    1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1};

  // Setting up sun light
  Light sunLight(glm::vec3(-1), glm::vec3(1, 1, 1), 1);

  // Create Scene
  activeScene = new Scene(
    &camera, glm::vec2(kWindowWidth, kWindowHeight), sunLight, ambientLight);
  activeScene->AddLight(new Light(
    glm::vec3(0, 3, 5),
    glm::vec3(1, 0, 0),
    25)); // Red, pointed at front model
  activeScene->AddLight(new Light(
    glm::vec3(0, 3, -5),
    glm::vec3(1, 1, 1),
    25)); // White, pointed at back of model
  activeScene->AddLight(new Light(
    glm::vec3(-5, 3, 0), glm::vec3(0, 1, 0), 25)); // Green, pointed from left
  activeScene->AddLight(new Light(
    glm::vec3(5, 3, 0), glm::vec3(0, 0, 1), 25)); // Blue, pointed from right


  // Load Models
  soulspearMesh.InitialiseFromFile("./Models/soulspear.obj");
  soulspearMesh.LoadMaterial("./Models/soulspear.mtl");
  bunnyMesh.InitialiseFromFile("./Models/bunny.obj");
  bunnyMesh.LoadMaterial("./Models/bunny.mtl");
  lucyMesh.InitialiseFromFile("./Models/Lucy.obj");
  lucyMesh.LoadMaterial("./Models/Lucy.mtl");

  // Create instances
  Instance* quadInstance =
    new Instance(quadTransform, &quadMesh, &simplePhongShader);
  Instance* soulspearInstance =
    new Instance(soulspearTransform, &soulspearMesh, &normalPhongShader);
  Instance* bunnyInstance =
    new Instance(bunnyTransform, &bunnyMesh, &phongNoTextureShader);
  Instance* lucyInstance =
    new Instance(lucyTransform, &lucyMesh, &phongNoTextureShader);

  // Add Instances to Scene
  activeScene->AddInstance(quadInstance);
  // activeScene->AddInstance(soulspearInstance);
  // activeScene->AddInstance(bunnyInstance);
  activeScene->AddInstance(lucyInstance);

  return true;
}

// Set Mouse Position Function
void Application::SetMousePosition(GLFWwindow* window, double x, double y)
{
  instance->mousePosition.x = (float)x;
  instance->mousePosition.y = (float)y;
}

bool Application::Update()
{
  camera.Update(0.1f, window);
  lastMousePosition = mousePosition;

  activeScene->Update();

  // Query time since application started
  float time = glfwGetTime();

  // Close window on ESC or pressing X in top right
  if (
    glfwWindowShouldClose(window) == true ||
    glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
    return false;
  }

  return true;
}

void Application::Draw()
{
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  glm::mat4 pv = camera.GetProjectionMatrix(kWindowWidth, kWindowHeight) *
                 camera.GetViewMatrix();

  aie::Gizmos::clear();

  aie::Gizmos::addTransform(glm::mat4(1));

  ///// Everything goes below here related to drawing /////

  // Create 10x10 grid
  CreateGrid();

  aie::Gizmos::draw(activeScene->GetProjectionView());

  // Idealy, a MeshRenderer would handle working out which shader is being used,
  // it would bundle all that are using the same one together, process those and
  // then do the next batch, etc. (Check Week 05 to see what he said around the
  // 50 minute mark)

  activeScene->Draw();

  // ImGui::Render(); // Draw this last to render over the top

  glfwSwapBuffers(window); // Draw the front, then the back, and alternate.
  glfwPollEvents(); // Windows related stuff - moving the window, etc.
}

void Application::Shutdown()
{
  aie::ImGui_Shutdown();

  delete activeScene;

  aie::Gizmos::destroy();
  glfwDestroyWindow(window);
  glfwTerminate();
}

/// <summary>
/// Creates a 10x10 grid of black lines with the centre
/// crossed lines being white.
/// </summary>
void Application::CreateGrid() const
{
  for (int i = 0; i < 21; i++) {
    aie::Gizmos::addLine(
      glm::vec3(-10 + i, 0, 10),
      glm::vec3(-10 + i, 0, -10),
      i == 10 ? kWhite : kBlack);
    aie::Gizmos::addLine(
      glm::vec3(10, 0, -10 + i),
      glm::vec3(-10, 0, -10 + i),
      i == 10 ? kWhite : kBlack);
  }
}
