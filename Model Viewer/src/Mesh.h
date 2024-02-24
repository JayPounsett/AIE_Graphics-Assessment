#pragma once
#include "glm/glm.hpp"

// Mesh class that hold mesh data (ibo vao etc.) then a mesh renderer that
// actually handles rendering of the mesh

class Mesh {

public:
  struct Vertex {
    glm::vec4 position;
    glm::vec4 normal;
  };

  Mesh() : m_triCount(0), m_vao(0), m_vbo(0), m_ibo(0) {}
  virtual ~Mesh();

  void initialiseQuad();
  virtual void draw();

protected:
  const unsigned int m_triCount = 2;

  // Vertex Array Object, Vertex Buffer Object, Index Buffer Object
  unsigned int m_vao, m_vbo, m_ibo;
};
