#pragma once

#include "GLFW/glfw3.h"
#include "glad.h"
#include "glm/glm.hpp"
#include "glm/ext.hpp"

class Camera
{
private:
    // Two angles and a position.
    // The angles are used for the forward of the camera.
    float theta; // Looking left and right
    float phi; // Looking up and down
    glm::vec3 position;
    float turnSpeed;

public:
    // Camera's default starting position
    Camera()
        : Camera(0, 0, { -10, 3, 0 }, 0.07f)
    {
    }
    Camera(float sTheta, float sPhi, glm::vec3 sPosition, float ts)
        : theta(sTheta)
        , phi(sPhi)
        , position(sPosition)
        , turnSpeed(ts)
    {
    }

    ~Camera();

    const glm::vec3 GetPosition() const { return position; }
    glm::mat4 GetViewMatrix() const;
    glm::mat4 GetProjectionMatrix(float w, float h) const;

    void Update(float dt, GLFWwindow* window);

private:
    float fov90 = glm::pi<float>() * 0.25f;
    float nearClip = 0.01f;
    float farClip = 1000.f;

    float thetaR = 0.f;
    float phiR = 0.f;

    glm::vec3 forward = { 0, 0, 0 };
    glm::vec3 right = { 0, 0, 0 };
    glm::vec3 up = { 0, 1, 0 };

public:
    float GetThetaRadians() const { return glm::radians(theta); }
    float GetPhiRadians() const { return glm::radians(phi); }

    /// <summary>
    /// Returns cos(phiR) * cos(thetaR), sin(phiR), cos(phiR) * sin(thetaR)
    /// </summary>
    /// <returns></returns>
    glm::vec3 GetForward() const
    {
        return { cos(phiR) * cos(thetaR), sin(phiR), cos(phiR) * sin(thetaR) };
    }

    /// <summary>
    /// Returns -sin(thetaR), 0, cos(thetaR)
    /// </summary>
    /// <returns></returns>
    glm::vec3 GetRight() const { return { -sin(thetaR), 0, cos(thetaR) }; }

    /// <summary>
    /// Returns Y axis
    /// </summary>
    /// <returns></returns>
    glm::vec3 GetUp() const { return { 0, 1, 0 }; }
};
