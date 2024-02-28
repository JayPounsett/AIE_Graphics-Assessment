#pragma once
#include "glm/glm.hpp"

class Camera;
class Light;
class Mesh;
class Scene;

namespace aie {
class ShaderProgram;
};

class Instance {

protected:
    glm::mat4 m_transform;
    Mesh* m_mesh;
    aie::ShaderProgram* m_shader;

public:
    Instance(glm::mat4 transform, Mesh* mesh, aie::ShaderProgram* shader)
        : m_transform(transform)
        , m_mesh(mesh)
        , m_shader(shader) { }
    Instance(Mesh* mesh, aie::ShaderProgram* shader) : m_mesh(mesh), m_shader(shader) {
        MakeTransform(glm::vec3(0), glm::vec3(0), glm::vec3(1));
    }
    Instance(glm::vec3 position, glm::vec3 rotation, glm::vec3 scale, glm::mat4 transform,
             Mesh* mesh, aie::ShaderProgram* shader)
        : m_transform(transform)
        , m_mesh(mesh)
        , m_shader(shader) {
        MakeTransform(position, rotation, scale);
    }
    ~Instance();

    void Draw(Scene* scene);
    void MakeTransform(glm::vec3 position, glm::vec3 eulerAngles, glm::vec3 scale);
};
