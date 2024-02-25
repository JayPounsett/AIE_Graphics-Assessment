#pragma once

#include "GLFW/glfw3.h"
#include "glad.h"
#include "glm/glm.hpp"

class Camera {
private:
  // Two angles and a position.
  // The angles are used for the forward of the camera.
  float theta; // Looking left and right
  float phi;   // Looking up and down
  glm::vec3 position;
  float turnSpeed;

public:
  // Camera's default starting position
  Camera() : Camera(0, 0, {-10, 3, 0}, 0.07f) {}
  Camera(float sTheta, float sPhi, glm::vec3 sPosition, float ts)
      : theta(sTheta), phi(sPhi), position(sPosition), turnSpeed(ts) {}

  ~Camera();

  const glm::vec3 GetPosition() const { return position; }
  glm::mat4 GetViewMatrix();
  glm::mat4 GetProjectionMatrix(float w, float h);

  void Update(float dt, GLFWwindow *window);

private:
  float perspectiveFOV90 = 0.25f; // 90 degrees
  float perspectiveNear = 0.01f;
  float perspectiveFar = 1000.f;

  float thetaR;
  float phiR;

  glm::vec3 forward;
  glm::vec3 right;
  glm::vec3 up;

public:
  const float GetThetaRadians() const { return glm::radians(theta); }
  const float GetPhiRadians() const { return glm::radians(phi); }

  const glm::vec3 GetForward() const {
    return {cos(phiR) * cos(thetaR), sin(phiR), cos(phiR) * sin(thetaR)};
  }
  const glm::vec3 GetRight() const { return {-sin(thetaR), 0, cos(thetaR)}; }
  const glm::vec3 GetUp() const { return {0, 1, 0}; }
};
