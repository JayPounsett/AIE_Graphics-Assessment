#include "Application.h"

#include <iostream>

#include "Camera.h"
#include "Gizmos.h"
#include "Instance.h"
#include "Scene.h"
#include "imgui_glfw3.h"

Application* Application::instance;

bool Application::Startup() {
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

  //// Load shaders
  //// Shader List: simpleShader, phongShader, phongNoTexture,
  /// normalPhongShader, reflectionShader
  // simpleShader.loadShader(
  //   aie::eShaderStage::VERTEX, "./shaders/vertex/simpleShader.vert");
  // simpleShader.loadShader(
  //   aie::eShaderStage::FRAGMENT, "./shaders/fragment/simpleShader.frag");

   //simplePhongShader.loadShader(
   //  aie::eShaderStage::VERTEX, "./shaders/vertex/phongShader.vert");
   //simplePhongShader.loadShader(
   //  aie::eShaderStage::FRAGMENT, "./shaders/fragment/phongShader.frag");

  phongNoTextureShader.loadShader(
    aie::eShaderStage::VERTEX, "./shaders/vertex/phongNoTextureShader.vert");
  phongNoTextureShader.loadShader(
    aie::eShaderStage::FRAGMENT,
    "./shaders/fragment/phongNoTextureShader.frag");

  normalPhongShader.loadShader(
    aie::eShaderStage::VERTEX, "./shaders/vertex/normalPhongShader.vert");
  normalPhongShader.loadShader(
    aie::eShaderStage::FRAGMENT, "./shaders/fragment/normalPhongShader.frag");

  skyboxShader.loadShader(
    aie::eShaderStage::VERTEX, "./shaders/vertex/cubemapShader.vert");
  skyboxShader.loadShader(
    aie::eShaderStage::FRAGMENT, "./shaders/fragment/cubemapShader.frag");

  reflectionShader.loadShader(
    aie::eShaderStage::VERTEX, "./shaders/vertex/reflectionShader.vert");
  reflectionShader.loadShader(
    aie::eShaderStage::FRAGMENT, "./shaders/fragment/reflectionShader.frag");

#pragma region Linking Shaders
  // if (simpleShader.link() == false)
  //{
  //   printf("Shader Error: %s\n", simpleShader.getLastError());
  //   return false;
  // }

  // if (simplePhongShader.link() == false)
  //{
  //   printf("Shader Error: %s\n", simplePhongShader.getLastError());
  //   return false;
  // }

  if (phongNoTextureShader.link() == false) {
    printf("Shader Error: %s\n", phongNoTextureShader.getLastError());
    return false;
  }

  if (normalPhongShader.link() == false) {
    printf("Shader Error: %s\n", normalPhongShader.getLastError());
    return false;
  }

  if (skyboxShader.link() == false) {
    printf("Shader Error: %s\n", skyboxShader.getLastError());
    return false;
  }

  if (reflectionShader.link() == false) {
    printf("Shader Error: %s\n", reflectionShader.getLastError());
    return false;
  }
#pragma endregion

  glm::mat4 dragonTransform = {
    1.5, 0, 0, 0, 0, 1.5, 0, 0, 0, 0, 1.5, 0, 0, 0, -20, 1};
  
  glm::mat4 soulspearTransform1 = {
    1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, -5, 0.5, 0, 1};

  glm::mat4 soulspearTransform2 = {
    1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 5, 0.5, 0, 1};

  glm::mat4 mirrorCubeTransform = {
    1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, mirrorCubeTransform[1].y, 0, 1};

  // Setting up the skybox
  std::vector<std::string> faces{
    "./textures/skybox/stormydays_ft.tga",
    "./textures/skybox/stormydays_bk.tga",
    "./textures/skybox/stormydays_up.tga",
    "./textures/skybox/stormydays_dn.tga",
    "./textures/skybox/stormydays_rt.tga",
    "./textures/skybox/stormydays_lf.tga"};

  skybox.InitialiseCubeMap();
  cubemapTexture = skybox.LoadCubeMap(faces);

  // Mirror Cube
  mirrorCube.InitialiseCube();

  // Setting up sun light
  Light sunLight(glm::vec3(-1), glm::vec3(0, 0, 0), 1);
  ambientLight = {0.25, 0.25, 0.25};

  // Create Scene
  activeScene = new Scene(
    &camera,
    glm::vec2(kWindowWidth, kWindowHeight),
    sunLight,
    ambientLight,
    cubemapTexture);

  // Adding point lights
  // Left Model
  activeScene->AddLight(new Light(
    glm::vec3(-5, 3, 5), glm::vec3(1, 0, 0), 50)); // Red, front of left model
  activeScene->AddLight(new Light(
    glm::vec3(-5, 3, -5), glm::vec3(0, 1, 0), 50)); // Green, back of left model

  // Right Model
  activeScene->AddLight(new Light(
    glm::vec3(5, 3, 5), glm::vec3(0.4, 0, 1), 50)); // Purple, front of left model
  activeScene->AddLight(new Light(
    glm::vec3(5, 3, -5),
    glm::vec3(1, 1, 0.8),
    50)); // Yellow, back of left model


  // Load Models
  dragonMesh.InitialiseFromFile("./models/dragon.obj");
  dragonMesh.LoadMaterial("./models/dragon.mtl");
  soulspearMesh.InitialiseFromFile("./models/soulspear.obj");
  soulspearMesh.LoadMaterial("./models/soulspear.mtl");

  Instance* dragonInstance = new Instance(
    dragonTransform, &dragonMesh, &reflectionShader, "Dragon");

  Instance* soulspearInstance1 = new Instance(
    soulspearTransform1, &soulspearMesh, &normalPhongShader, "Soulspear (Left)");

  Instance* soulspearInstance2 = new Instance(
    soulspearTransform2, &soulspearMesh, &normalPhongShader, "Soulspear (Right)");

  Instance* mirrorCubeInstance =
    new Instance(mirrorCubeTransform, &mirrorCube, &reflectionShader, "Mirror Cube");

  // Add Instances to Scene
  activeScene->AddInstance(dragonInstance);
  activeScene->AddInstance(mirrorCubeInstance);
  activeScene->AddInstance(soulspearInstance1);
  activeScene->AddInstance(soulspearInstance2);
  
  return true;
}

// Set Mouse Position Function
void Application::SetMousePosition(GLFWwindow* window, double x, double y) {
  instance->mousePosition.x = (float)x;
  instance->mousePosition.y = (float)y;
}

bool Application::Update() {
  camera.Update(0.1f, window);
  lastMousePosition = mousePosition;

  activeScene->Update();

  // Close window on ESC or pressing X in top right
  if (
    glfwWindowShouldClose(window) == true ||
    glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
    return false;
  }

  return true;
}

void Application::Draw() {
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  glm::mat4 pv = camera.GetProjectionMatrix(kWindowWidth, kWindowHeight) *
                 camera.GetViewMatrix();

  aie::Gizmos::clear();

  aie::Gizmos::addTransform(glm::mat4(1));

  ///// Everything goes below here related to drawing /////

  //// Create 10x10 grid
  // CreateGrid();

  aie::Gizmos::draw(activeScene->GetProjectionView());

  // Create & Draw Skybox
  skyboxShader.bind();
  skyboxShader.bindUniform("Projection", activeScene->GetProjectionMatrix());
  skyboxShader.bindUniform("View", activeScene->GetViewMatrix());

  glBindTexture(GL_TEXTURE_CUBE_MAP, cubemapTexture);
  skybox.Draw();
  glDepthMask(true);

  activeScene->Draw();

  glfwSwapBuffers(window); // Draw the front, then the back, and alternate.
  glfwPollEvents(); // Windows related stuff - moving the window, etc.
}

void Application::Shutdown() {
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
void Application::CreateGrid() const {
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
