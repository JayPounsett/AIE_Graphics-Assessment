#include "Scene.h"

#include <memory>

#include "Camera.h"
#include "Instance.h"
#include "imgui_glfw3.h"

Scene::~Scene()
{
  for (auto it = instances.begin(); it > instances.end();) {
    delete *it;
  }

  // for (auto it = instances.begin(); it > instances.end();) {
  //   delete (*it->get());
  // }
}

void Scene::AddInstance(Instance* instance) { instances.push_back(instance); }
// void Scene::AddInstance(std::unique_ptr<Instance> instance)
//{
//   instances.push_back(instance);
// }

void Scene::Update()
{
  pv = camera->GetProjectionMatrix(windowSize.x, windowSize.y) *
       camera->GetViewMatrix();

  aie::ImGui_NewFrame();
  ImGui::Begin("Light Settings");
  ImGui::DragFloat3(
    "Sunlight Direction", &light.direction[0], 0.1f, -1.0f, 1.0f);
  ImGui::DragFloat3("Sunlight Colour", &light.colour[0], 0.1f, 0.0f, 2.0f);
  // Add in non-interactive variables to show the coordinates of the camera
  // (position, theta, phi)
  ImGui::End();
}

void Scene::Draw()
{
  for (auto it = instances.begin(); it != instances.end(); it++) {
    (*it)->Draw(this);
    // for (auto it = instances.begin(); it != instances.end(); it++) {
    //   if ((*it)->CheckDoesInstanceExist()) {
    //     (*it)->Draw(this);
    // }
  }
}
