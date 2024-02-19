#include "MeshRenderer.h"
#include <assert.h>

void MeshRenderer::InitialiseQuad() {
  // Check that the mesh is not already initialised
  assert(m_vao == 0);

  // Generate buffers
  glGenBuffers(1, &m_vbo);
  glGenVertexArrays(1, &m_vao);

  // Bind vertex array -> mesh wrapper
  glBindVertexArray(m_vao);

  // Bind vertex buffer
  glBindBuffer(GL_ARRAY_BUFFER, m_vbo);

  // Defining two triangles for a quad
  Vertex vertices[6];
  vertices[0].m_Position = {-0.5f, 0, 0.5f, 1};
  vertices[1].m_Position = {0.5f, 0, 0.5f, 1};
  vertices[2].m_Position = {-0.5f, 0, -0.5f, 1};

  vertices[3].m_Position = {-0.5f, 0, 0.5f, 1};
  vertices[4].m_Position = {0.5f, 0, 0.5f, 1};
  vertices[5].m_Position = {0.5f, 0, -0.5f, 1};

  // Fill the vertex buffer
  glBufferData(GL_ARRAY_BUFFER, 6 * sizeof(Vertex), vertices, GL_STATIC_DRAW);

  // Enable first element as position
  glDisableVertexAttribArray(0);
  glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), 0); 
  
  // -= glVertexAttribPointer =-
  // It will start at the first element (0), read in 4 floats, 
  // step the size of the vertex over and then start again.

  glBindVertexArray(0);
  glBindBuffer(GL_ARRAY_BUFFER, 0);
  
  //Quad has 2 triangles
  m_TriCount = 2;
}

void MeshRenderer::Draw() {}
