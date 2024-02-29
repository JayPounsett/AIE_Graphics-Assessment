#include "Scene.h"

#include "Camera.h"
#include "Instance.h"
#include "imgui_glfw3.h"

void Scene::AddInstance(Instance* instance) { instances.push_back(instance); }

Scene::~Scene()
{
  for (auto it = instances.begin(); it > instances.end();) {
    delete *it;
  }
}

void Scene::Update()
{
  pv = camera->GetProjectionMatrix(windowSize.x, windowSize.y) *
       camera->GetViewMatrix();

  aie::ImGui_NewFrame();
  ImGui::Begin("Light Settings");
  ImGui::DragFloat3(
    "Sunlight Direction", &light.direction[0], 0.1f, -1.0f, 1.0f);
  ImGui::DragFloat3("Sunlight Colour", &light.colour[0], 0.1f, 0.0f, 2.0f);
  ImGui::End();
}

void Scene::Draw()
{
  for (auto it = instances.begin(); it != instances.end(); it++) {
    (*it)->Draw(this);
  }
}
