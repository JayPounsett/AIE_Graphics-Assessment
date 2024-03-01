#pragma once
#include <vector>
#include <memory>
#include "glm/glm.hpp"

struct Light
{
  glm::vec3 direction;
  glm::vec3 colour;
};

class Camera;
class Instance;
class Light;

class Scene
{
public:
  Scene(
    Camera* camera, glm::vec2 windowSize, Light& light, glm::vec3 ambientLight)
    : camera(camera),
      windowSize(windowSize),
      light(light),
      ambientLight(ambientLight)
  {
  }
  ~Scene();
  void AddInstance(Instance* instance);
  //void AddInstance(std::unique_ptr<Instance> instance);

  void RemoveInstance(Instance* instance);

  Camera* GetCamera() { return camera; }

  // Check video at 3h52m on the consts
  const glm::vec2 GetWindowSize() const { return windowSize; }
  const glm::mat4 GetProjectionView() const { return pv; }
  Light GetLight() const { return light; }
  const glm::vec3 GetAmbientLight() const { return ambientLight; }

  void Update(); // Good habit to pass in float dt, check into it in my own time
                 // as dt is elsewhere
  void Draw();

protected:
  Camera* camera;
  glm::vec2 windowSize;
  Light light;

  glm::vec3 ambientLight;
  std::vector<Instance*> instances;
  //std::vector<std::unique_ptr<Instance>> instances;

  glm::mat4 pv;
};
