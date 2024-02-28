#pragma once
#include "glm/glm.hpp"
#include <vector>

struct Light {
    glm::vec3 direction;
    glm::vec3 colour;
};

class Camera;
class Instance;
class Light;

class Scene {
protected:
    Camera* m_camera;
    glm::vec2 m_windowSize;
    Light m_light;

    glm::vec3 m_ambientLight;
    std::vector<Instance*> m_instances;

    glm::mat4 pv;

public:
    Scene(Camera* camera, glm::vec2 windowSize, Light& light, glm::vec3 ambientLight)
        : m_camera(camera)
        , m_windowSize(windowSize)
        , m_light(light)
        , m_ambientLight(ambientLight) { }
    ~Scene();
    void AddInstance(Instance* instance);

    Camera* GetCamera() { return m_camera; }
    const glm::vec2 GetWindowSize() const {
        return m_windowSize;
    } // Check video at 3h52m on the consts
    const glm::mat4 GetProjectionView() const { return pv; }
    Light GetLight() { return m_light; }
    const glm::vec3 GetAmbientLight() const { return m_ambientLight; }

    void Update(); // Good habit to pass in float dt, check into it in my own time as dt is elsewhere
    void Draw();
};
