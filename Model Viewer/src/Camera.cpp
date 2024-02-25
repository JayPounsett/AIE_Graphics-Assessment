#include "camera.h"

#include "application.h"
#include "glm/ext.hpp"

Camera::~Camera() {}

glm::mat4 Camera::GetViewMatrix() {
  return glm::lookAt(position, position + forward,
                     glm::vec3(0, 1, 0)); // vec3 states Y axis is up
}

glm::mat4 Camera::GetProjectionMatrix(float w, float h) {
  return glm::perspective(glm::pi<float>() * perspectiveFOV90, w / h,
                          perspectiveNear, perspectiveFar);
}

void Camera::Update(float dt, GLFWwindow *window) {
  thetaR = GetThetaRadians();
  phiR = GetPhiRadians();

  forward = GetForward();
  right = GetRight();
  up = GetUp();

#pragma region Keyboard Inputs
  if (glfwGetKey(window, GLFW_KEY_W)) {
    position += forward * dt;
  }

  if (glfwGetKey(window, GLFW_KEY_S)) {
    position -= forward * dt;
  }

  if (glfwGetKey(window, GLFW_KEY_A)) {
    position -= right * dt;
  }

  if (glfwGetKey(window, GLFW_KEY_D)) {
    position += right * dt;
  }

  if (glfwGetKey(window, GLFW_KEY_Z)) {
    position += up * dt;
  }

  if (glfwGetKey(window, GLFW_KEY_X)) {
    position -= up * dt;
  }
#pragma endregion

#pragma region Mouse Input
  glm::vec2 mouseDelta = Application::get()->getMouseDelta();

  // if the right button is down, increment theta and phi
  if (glfwGetMouseButton(window, 1)) {
    theta += turnSpeed * mouseDelta.x;
    phi -= turnSpeed * mouseDelta.y;
  }
#pragma endregion
}
