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
    glm::vec3 ambientLight,
    unsigned int skyboxTextureID)
    : camera{camera},
      windowSize{windowSize},
      sunLight{sunLight},
      ambientLight{ambientLight},
      skyboxTextureID(skyboxTextureID)
  {
  }

  ~Scene();
  void AddInstance(Instance* instance);
  void AddLight(Light* light);

  Camera* GetCamera() { return camera; }

    // Check video at 3h52m on the consts
  const glm::vec2 GetWindowSize() const { return windowSize; }
  const glm::mat4 GetProjectionView() const { return pv; }
  const glm::mat4 GetProjectionMatrix() const;
  const glm::mat4 GetViewMatrix() const;
  Light GetLight() const { return sunLight; }
  glm::vec3 GetAmbientLight() { return ambientLight; }

  int GetNumberLights() { return (int)pointLights.size(); }
  glm::vec3* GetPointPositions() { return &pointLightPositions[0]; }
  glm::vec3* GetPointColours() { return &pointLightColours[0]; }
  unsigned int GetSkyboxTextureID() const { return skyboxTextureID; }

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

  unsigned int skyboxTextureID;

  glm::mat4 pv;
};
