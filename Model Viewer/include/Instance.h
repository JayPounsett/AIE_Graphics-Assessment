#pragma once
#include "Application.h"
#include "Mesh.h"
#include "glm/ext.hpp"

struct Light
{
    glm::vec3 direction;
    glm::vec3 colour;
};

class Instance
{
public:
    void Draw(Camera* camera, float windowWidth, float windowHeight, glm::vec3 ambientLight,
              Light* light);

protected:
    glm::mat4 m_transform;
    Mesh* m_mesh;
    aie::ShaderProgram* m_shader;
};
