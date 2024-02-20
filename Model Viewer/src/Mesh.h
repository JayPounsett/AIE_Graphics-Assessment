#pragma once
#include "glad.h"

// Mesh class that hold mesh data (ibo vao etc.) then a mesh renderer that
// actually handles rendering of the mesh

class Mesh {
public:
  Mesh() : tri_count(0), vao(0), vbo(0), ibo(0){}
  virtual ~Mesh();

protected:
  unsigned int tri_count;

  // Vertex Array Object, Vertex Buffer Object, Index Buffer Object
  unsigned int vao, vbo, ibo;
};
