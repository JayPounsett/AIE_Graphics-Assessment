#pragma once
#include "glm/ext.hpp"
#include "glm/glm.hpp"
#include <vector>
#include <string>

class Camera;
class Light;
class Mesh;
class Scene;

namespace aie {
class ShaderProgram;
};

class Instance {
public:
  Instance(
    glm::mat4 transform,
    Mesh* mesh,
    aie::ShaderProgram* shader,
    std::string instanceName = "No Name")
    : transform(transform), mesh(mesh), shader(shader), instanceName(instanceName) {}
  Instance(
    Mesh* mesh,
    aie::ShaderProgram* shader,
    std::string instanceName = "No Name")
    : mesh(mesh), shader(shader), instanceName(instanceName) {
    MakeTransform(glm::vec3(0), glm::vec3(0), glm::vec3(1));
  }
  Instance(
    glm::vec3 position,
    glm::vec3 rotation,
    glm::vec3 scale,
    glm::mat4 transform,
    Mesh* mesh,
    aie::ShaderProgram* shader,
    std::string instanceName = "No Name")
    : transform(transform), mesh(mesh), shader(shader), instanceName(instanceName) {
    MakeTransform(position, rotation, scale);
  }
  ~Instance();

  void Draw(Scene* scene);
  void MakeTransform(
    glm::vec3 position, glm::vec3 eulerAngles, glm::vec3 scale);

  glm::mat4 GetTransform() { return this->transform; }

  std::string GetInstanceName() { return this->instanceName; }

  glm::mat4 RotateModel(float angle, glm::vec3 rotationAxis) {
    return this->transform = glm::rotate(
             this->transform, glm::radians(angle), rotationAxis);
  }

  float& GetAngle() { return angle; }

protected:
  glm::mat4 transform;
  Mesh* mesh;
  aie::ShaderProgram* shader;
    
  float angle = 0.0f;

  std::string instanceName = "No Name";
};
