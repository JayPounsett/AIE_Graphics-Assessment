#pragma once
#include "glm/glm.hpp"

class Camera;
class Light;
class Mesh;
class Scene;

namespace aie {
class ShaderProgram;
};

class Instance
{
protected:
  glm::mat4 transform;
  Mesh* mesh;
  aie::ShaderProgram* shader;

public:
  Instance(glm::mat4 transform, Mesh* mesh, aie::ShaderProgram* shader)
    : transform(transform), mesh(mesh), shader(shader)
  {
  }
  Instance(Mesh* mesh, aie::ShaderProgram* shader) : mesh(mesh), shader(shader)
  {
    MakeTransform(glm::vec3(0), glm::vec3(0), glm::vec3(1));
  }
  Instance(
    glm::vec3 position,
    glm::vec3 rotation,
    glm::vec3 scale,
    glm::mat4 transform,
    Mesh* mesh,
    aie::ShaderProgram* shader)
    : transform(transform), mesh(mesh), shader(shader)
  {
    MakeTransform(position, rotation, scale);
  }
  ~Instance();

  void Draw(Scene* scene);
  void MakeTransform(
    glm::vec3 position, glm::vec3 eulerAngles, glm::vec3 scale);
};
