#include "Scene.h"

#include <memory>

#include "Camera.h"
#include "Instance.h"
#include "Mesh.h"
#include "imgui_glfw3.h"

Scene::~Scene() {
  for (auto it = instances.begin(); it != instances.end(); it++) {
    delete *it;
  }

  for (auto it = pointLights.begin(); it != pointLights.end(); it++) {
    delete *it;
  }
}
void Scene::AddInstance(Instance* instance) { instances.push_back(instance); }

void Scene::AddLight(Light* light) { pointLights.push_back(light); }

const glm::mat4 Scene::GetProjectionMatrix() const {
  return camera->GetProjectionMatrix(windowSize.x, windowSize.y);
}

const glm::mat4 Scene::GetViewMatrix() const {
  return glm::mat4(glm::mat3(camera->GetViewMatrix()));
}

void Scene::Update() {
  pv = camera->GetProjectionMatrix(windowSize.x, windowSize.y) *
       camera->GetViewMatrix();

  for (int i = 0; i < instances.size(); i++) {
    this->instances[i]->RotateModel(instances[i]->GetAngle(), rotationAxis);
  }
}

void Scene::Draw() {
  pv = camera->GetProjectionMatrix(windowSize.x, windowSize.y) *
       camera->GetViewMatrix();

  for (int i = 0; i < pointLights.size(); i++) {
    pointLightPositions[i] = pointLights[i]->direction;
    pointLightColours[i] = pointLights[i]->colour;
  }

  for (auto it = instances.begin(); it != instances.end(); it++) {
    (*it)->Draw(this);
  }

  aie::ImGui_NewFrame();
  ImGui::Begin("Light Settings");
  ImGui::DragFloat3(
    "Sunlight Direction", &sunLight.direction[0], 0.1f, -1.0f, 1.0f);
  ImGui::DragFloat3("Sunlight Colour", &sunLight.colour[0], 0.1f, 0.0f, 2.0f);

  // UI to Rotate Model
   for (int i = 0; i < instances.size(); i++) {
    ImGui::SliderFloat(
      ("Rotate " + instances[i]->GetInstanceName()).c_str(),
      &instances[i]->GetAngle(),
      -1.0f,
      1.0f);
  }

  ImGui::End();
  ImGui::Render();
}
