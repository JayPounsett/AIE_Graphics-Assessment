#include "mesh_renderer.h"
#include <assert.h>

void MeshRenderer::InitialiseQuad() {
  // Check that the mesh is not already initialised
  assert(vao == 0);

  // Generate buffers
  glGenBuffers(1, &vbo);
  glGenVertexArrays(1, &vao);

  // Bind vertex array -> mesh wrapper
  glBindVertexArray(vao);

  // Bind vertex buffer
  glBindBuffer(GL_ARRAY_BUFFER, vbo);

  // Defining two triangles for a quad
  Vertex vertices[6];

  vertices[0].position = {-0.5f, 0, 0.5f, 1};
  vertices[1].position = {0.5f, 0, 0.5f, 1};
  vertices[2].position = {-0.5f, 0, -0.5f, 1};

  vertices[3].position = {-0.5f, 0, 0.5f, 1};
  vertices[4].position = {0.5f, 0, 0.5f, 1};
  vertices[5].position = {0.5f, 0, -0.5f, 1};

  // Fill the vertex buffer
  glBufferData(GL_ARRAY_BUFFER, 6 * sizeof(Vertex), vertices, GL_STATIC_DRAW);

  // Enable first element as position
  glDisableVertexAttribArray(0);
  glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), 0);

  // Note to Future Self: glVertexAttribPointer
  // It will start at the first element (0), read in 4 floats,
  // step the size of the vertex over and then start again.

  // Unbind buffers
  glBindVertexArray(0);
  glBindBuffer(GL_ARRAY_BUFFER, 0);

  // Quad has 2 triangles
  tri_count = 2;
}

void MeshRenderer::Draw() { 
    glBindVertexArray(vao);


}
