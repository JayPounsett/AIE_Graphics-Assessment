#include "camera.h"

#include "application.h"
#include "glm/ext.hpp"

Camera::~Camera() { }

glm::mat4 Camera::GetViewMatrix() const
{
    return glm::lookAt(position, position + forward, up);
}

glm::mat4 Camera::GetProjectionMatrix(float w, float h) const
{
    return glm::perspective(fov90, w / h, nearClip, farClip);
}

void Camera::Update(float dt, GLFWwindow* window)
{
    thetaR = GetThetaRadians();
    phiR = GetPhiRadians();

    forward = GetForward();
    right = GetRight();
    up = GetUp();

    if (glfwGetKey(window, GLFW_KEY_W))
    {
        position += forward * dt;
    }

    if (glfwGetKey(window, GLFW_KEY_S))
    {
        position -= forward * dt;
    }

    if (glfwGetKey(window, GLFW_KEY_A))
    {
        position -= right * dt;
    }

    if (glfwGetKey(window, GLFW_KEY_D))
    {
        position += right * dt;
    }

    if (glfwGetKey(window, GLFW_KEY_Z))
    {
        position += up * dt;
    }

    if (glfwGetKey(window, GLFW_KEY_X))
    {
        position -= up * dt;
    }

    glm::vec2 mouseDelta = Application::Get()->GetMouseDelta();

    // if the right button is down, increment theta and phi
    if (glfwGetMouseButton(window, 1))
    {
        theta += turnSpeed * mouseDelta.x;
        phi -= turnSpeed * mouseDelta.y;
    }
}
