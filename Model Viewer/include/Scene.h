#pragma once
#include <memory>
#include <vector>

#include "glm/glm.hpp"

struct Light
{
  glm::vec3 direction;
  glm::vec3 colour;

  Light(glm::vec3 position, glm::vec3 colour, float intensity)
    : direction(position)
  {
    this->colour = colour * intensity;
  }
};

class Camera;
class Instance;
class Light;

class Scene
{
public:
  Scene(
    Camera* camera,
    glm::vec2 windowSize,
    Light& sunLight,
    glm::vec3 ambientLight)
    : camera(camera),
      windowSize(windowSize),
      sunLight(sunLight),
      ambientLight(ambientLight)
  {
  }

  ~Scene();
  void AddInstance(Instance* instance);
  void AddLight(Light* light);
  // void RemoveInstance(Instance* instance);

  Camera* GetCamera() { return camera; }

  // Check video at 3h52m on the consts
  const glm::vec2 GetWindowSize() const { return windowSize; }
  const glm::mat4 GetProjectionView() const { return pv; }
  Light GetLight() const { return sunLight; }
  glm::vec3 GetAmbientLight() { return ambientLight; }

  int GetNumberLights() { return (int)pointLights.size(); }
  glm::vec3* GetPointPositions() { return &pointLightPositions[0]; }
  glm::vec3* GetPointColours() { return &pointLightColours[0]; }

  void Update(); // Good habit to pass in float dt, check into it in my own time
                 // as dt is elsewhere
  void Draw();

protected:
  static const int MAX_LIGHTS = 4;

  Camera* camera;
  glm::vec2 windowSize;

  glm::vec3 ambientLight;
  Light sunLight;
  std::vector<Light*> pointLights;
  glm::vec3 pointLightPositions[MAX_LIGHTS];
  glm::vec3 pointLightColours[MAX_LIGHTS];

  std::vector<Instance*> instances;

  glm::mat4 pv;
};
