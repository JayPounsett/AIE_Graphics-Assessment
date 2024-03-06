#include "Instance.h"

#include "Application.h"
#include "Camera.h"
#include "Mesh.h"
#include "Scene.h"
#include "Shader.h"

Instance::~Instance() {}

void Instance::Draw(Scene* scene)
{
  // Bind shader
  shader->bind();

  // Bind lighting
  shader->bindUniform("AmbientColour", scene->GetAmbientLight());
  shader->bindUniform("LightColour", scene->GetLight().colour);
  shader->bindUniform("LightDirection", scene->GetLight().direction);

  // Bind camera position
  shader->bindUniform("CameraPosition", scene->GetCamera()->GetPosition());

  // Bind our point lights
  int numLights = scene->GetNumberLights();
  shader->bindUniform("numLights", numLights);
  shader->bindUniform(
    "PointLightPosition", numLights, scene->GetPointPositions());
  shader->bindUniform("PointLightColour", numLights, scene->GetPointColours());

  // Bind transform
  auto pvm = scene->GetProjectionView() * transform;
  shader->bindUniform("ProjectionViewModel", pvm);
  shader->bindUniform("ModelMatrix", transform);

  mesh->ApplyMaterial(shader);

  // Draw
  mesh->Draw();
}

void Instance::MakeTransform(
  glm::vec3 position, glm::vec3 eulerAngles, glm::vec3 scale)
{
  transform =
    glm::translate(glm::mat4(1), position) *
    glm::rotate(glm::mat4(1), glm::radians(eulerAngles.z), glm::vec3(0, 0, 1)) *
    glm::rotate(glm::mat4(1), glm::radians(eulerAngles.y), glm::vec3(0, 1, 0)) *
    glm::rotate(glm::mat4(1), glm::radians(eulerAngles.x), glm::vec3(1, 0, 0)) *
    glm::scale(glm::mat4(1), scale);
}
