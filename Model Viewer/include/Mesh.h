#pragma once
#include <vector>

#include "Texture.h"
#include "glm/glm.hpp"

// Mesh class that hold mesh data (ibo vao etc.) then a mesh renderer that
// actually handles rendering of the mesh

namespace aie {
class ShaderProgram;
}

class Mesh {
public:
  struct Vertex {
    glm::vec4 position;
    glm::vec4 normal;
    glm::vec2 texCoord;
    glm::vec4 tangent;
  };

  Mesh()
    : triCount(0),
      vao(0),
      vbo(0),
      ibo(0),
      Ka{0, 0, 0},
      Kd{0, 0, 0},
      Ks{0, 0, 0},
      specularPower(32.0f){}

  virtual ~Mesh();

  void InitialiseCube();
  void InitialiseQuad();
  void Initialise(
    unsigned int vertexCount,
    const Vertex* vertices,
    unsigned int indexCount = 0,
    unsigned int* indices = nullptr);

  void InitialiseFromFile(const char* fileName);
  virtual void Draw();

  void ApplyMaterial(aie::ShaderProgram* shader);
  void LoadMaterial(const char* fileName);
  void CreateMaterial(
    glm::vec3 Ka,
    glm::vec3 Kd,
    glm::vec3 Ks,
    float specularPower,
    const char* filePath);

protected:
  void CalculateTangents(
    Vertex* vertices,
    unsigned int vertexCount,
    const std::vector<unsigned int>& indices);

protected:
  unsigned int triCount;

  // Vertex Array Object, Vertex Buffer Object, Index Buffer Object
  unsigned int vao, vbo, ibo;

  glm::vec3 Ka; // Ambient colour of the surface
  glm::vec3 Kd; // Diffuse colour of the surface
  glm::vec3 Ks; // Specular colour of the surface

  float specularPower = 32.0f; // Tightness of specular highlights

  aie::Texture mapKd; // Diffuse texture map
  aie::Texture mapKs; // Specular texture map
  aie::Texture mapBump; // Normal texture map

  float rotationAngle = 0.0f;

};
