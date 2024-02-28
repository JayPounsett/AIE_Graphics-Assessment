#include "Scene.h"
#include "Camera.h"
#include "Instance.h"
#include "imgui_glfw3.h"

void Scene::AddInstance(Instance* instance) { m_instances.push_back(instance); }

Scene::~Scene() {
    for (auto it = m_instances.begin(); it > m_instances.end();) { 
        delete *it;
    }
}

void Scene::Update() {
    pv = m_camera->GetProjectionMatrix(m_windowSize.x, m_windowSize.y) * m_camera->GetViewMatrix();

    aie::ImGui_NewFrame();
    ImGui::Begin("Light Settings");
    ImGui::DragFloat3("Sunlight Direction", &m_light.direction[0], 0.1f, -1.0f,
                      1.0f);
    ImGui::DragFloat3("Sunlight Colour", &m_light.colour[0], 0.1f, 0.0f, 2.0f);
    ImGui::End();
}

void Scene::Draw() {
    for (auto it = m_instances.begin(); it != m_instances.end(); it++) {
        (*it)->Draw(this);
    }
}
