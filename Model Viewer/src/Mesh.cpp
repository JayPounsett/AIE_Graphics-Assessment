#include "Mesh.h"

Mesh::~Mesh() {
  glDeleteVertexArrays(1, &m_vao);
  glDeleteBuffers(1, &m_vbo);
  glDeleteBuffers(1, &m_ibo);
}
