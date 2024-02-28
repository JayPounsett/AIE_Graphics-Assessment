#include "Instance.h"
#include "Application.h"
#include "Camera.h"
#include "Mesh.h"
#include "Scene.h"
#include "Shader.h"

Instance::~Instance() { }

void Instance::Draw(Scene* scene) {
    // Bind shader
    m_shader->bind();

    // Bind lighting
    m_shader->bindUniform("AmbientColour", scene->GetAmbientLight());
    m_shader->bindUniform("LightColour", scene->GetLight().colour);
    m_shader->bindUniform("LightDirection", scene->GetLight().direction);

    // Bind camera position
    m_shader->bindUniform("CameraPosition", scene->GetCamera()->GetPosition());

    // Bind transform
    auto pvm = scene->GetProjectionView() * m_transform;
    m_shader->bindUniform("ProjectionViewModel", pvm);
    m_shader->bindUniform("ModelMatrix", m_transform);

    m_mesh->ApplyMaterial(m_shader);

    // Draw
    m_mesh->Draw();
}

void Instance::MakeTransform(glm::vec3 position, glm::vec3 eulerAngles, glm::vec3 scale) {

    m_transform = glm::translate(glm::mat4(1), position)
        * glm::rotate(glm::mat4(1), glm::radians(eulerAngles.z), glm::vec3(0, 0, 1))
        * glm::rotate(glm::mat4(1), glm::radians(eulerAngles.y), glm::vec3(0, 1, 0))
        * glm::rotate(glm::mat4(1), glm::radians(eulerAngles.x), glm::vec3(1, 0, 0))
        * glm::scale(glm::mat4(1), scale);
}
