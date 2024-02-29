#pragma once

#include "GLFW/glfw3.h"
#include "glad.h"
#include "glm/ext.hpp"
#include "glm/glm.hpp"

class Camera
{
public:
  //Camera() : Camera(0, 0, {-10, 3, 0}, 0.07f) {} // Values given in lessons
  Camera() : Camera(-90, -20, {0, 6, 12}, 0.07f) {}
  Camera(float theta, float phi, glm::vec3 position, float ts)
    : theta(theta), phi(phi), position(position), turnSpeed(ts)
  {
  }

  ~Camera();

  void Update(float dt, GLFWwindow* window);

  float GetThetaRadians() const { return glm::radians(theta); }
  float GetPhiRadians() const { return glm::radians(phi); }

  glm::vec3 GetForward() const
  {
    return {cos(phiR) * cos(thetaR), sin(phiR), cos(phiR) * sin(thetaR)};
  }
  glm::vec3 GetRight() const { return {-sin(thetaR), 0, cos(thetaR)}; }
  glm::vec3 GetUp() const { return {0, 1, 0}; }

  const glm::vec3 GetPosition() const { return position; }
  glm::mat4 GetViewMatrix() const;
  glm::mat4 GetProjectionMatrix(float w, float h) const;

private:
  // Two angles and a position.
  // The angles are used for the forward of the camera.
  float theta; // Looking left and right
  float phi; // Looking up and down
  glm::vec3 position;

  float thetaR = 0.f;
  float phiR = 0.f;

  float turnSpeed;
  const float kFOV_90 = glm::pi<float>() * 0.25f;
  const float kNEAR_CLIP = 0.01f;
  const float kFAR_CLIP = 1000.f;


  glm::vec3 forward = {0, 0, 0};
  glm::vec3 right = {0, 0, 0};
  glm::vec3 up = {0, 1, 0};
};
