#pragma once
#include "glad.h"

// Mesh class that hold mesh data (ibo vao etc.) then a mesh renderer that
// actually handles rendering of the mesh

class Mesh {
public:
  Mesh() : m_TriCount(0), m_vao(0), m_vbo(0), m_ibo(0){}
  virtual ~Mesh();

protected:
  unsigned int m_TriCount;

  // Vertex Array Object, Vertex Buffer Object, Index Buffer Object
  unsigned int m_vao, m_vbo, m_ibo;
};