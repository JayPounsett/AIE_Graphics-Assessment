#include "Instance.h"

void Instance::Draw(Camera* camera, float windowWidth, float windowHeight, glm::vec3 ambientLight,
                    Light* light)
{
    // Set the shader pipeline
    m_shader->bind();

    // Bind transform and other uniforms
    auto pvm = camera->GetProjectionMatrix(windowWidth, windowHeight) * camera->GetViewMatrix()
        * m_transform;
    m_shader->bindUniform("ProjectionViewModel", pvm);
    m_shader->bindUniform("ModelMatrix", m_transform);
    m_shader->bindUniform("AmbientColour", ambientLight);
    m_shader->bindUniform("LightColour", light->colour);
    m_shader->bindUniform("LightDirection", light->direction);

    m_shader->bindUniform("CameraPosition", camera->GetPosition());

    // Draw mesh
    m_mesh->Draw();
}
