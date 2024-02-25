#pragma once
#include "glm/glm.hpp"

// Mesh class that hold mesh data (ibo vao etc.) then a mesh renderer that
// actually handles rendering of the mesh

namespace aie
{
class ShaderProgram;
}

class Mesh
{

public:
    struct Vertex
    {
        glm::vec4 position;
        glm::vec4 normal;
        glm::vec2 texCoord;
    };

    Mesh()
        : m_triCount(0)
        , vao(0)
        , vbo(0)
        , ibo(0)
    {
    }
    virtual ~Mesh();

    void InitialiseQuad();
    void Initialise(unsigned int vertexCount, const Vertex* vertices,
        unsigned int indexCount = 0, unsigned int* indices = nullptr);

    void InitialiseFromFile(const char* fileName);
    virtual void Draw();

    void ApplyMaterial(aie::ShaderProgram* shader);
    void LoadMaterial(const char* fileName);

protected:
    unsigned int m_triCount;

    // Vertex Array Object, Vertex Buffer Object, Index Buffer Object
    unsigned int vao, vbo, ibo;

    glm::vec3 Ka = { 0.25, 0.25, 0.25 }; // ambient colour of the surface
    glm::vec3 Kd = { 0.25, 0.25, 0.25 }; // diffuse colour of the surface
    glm::vec3 Ks = { 0.25, 0.25, 0.25 }; // specular colour of the surface

    float specularPower = 2; // tightness of specular highlights
};
