#include "Mesh.h"
#include "Shader.h"
#include "glad.h"
#include <assert.h>
#include <assimp/cimport.h>
#include <assimp/scene.h>
#include <vector>
#include <fstream>
#include <sstream>

Mesh::~Mesh() {
  glDeleteVertexArrays(1, &vao);
  glDeleteBuffers(1, &vbo);
  glDeleteBuffers(1, &ibo);
}

void Mesh::InitialiseQuad() {
  // check that the mesh is not initialised already
  assert(vao == 0);

  // generate buffers
  glGenBuffers(1, &vbo);
  glGenVertexArrays(1, &vao);

  // bind vortex array -> mesh wrapper
  glBindVertexArray(vao);

  // bind vertex buffer
  glBindBuffer(GL_ARRAY_BUFFER, vbo);

  // Creating two triangles to make a square
  Vertex vertices[6];
  vertices[0].position = {-0.5f, 0, 0.5f, 1};
  vertices[1].position = {0.5f, 0, 0.5f, 1};
  vertices[2].position = {-0.5f, 0, -0.5f, 1};

  vertices[3].position = {-0.5f, 0, -0.5f, 1};
  vertices[4].position = {0.5f, 0, 0.5f, 1};
  vertices[5].position = {0.5f, 0, -0.5f, 1};

  vertices[0].normal = {0, 1, 0, 0};
  vertices[1].normal = {0, 1, 0, 0};
  vertices[2].normal = {0, 1, 0, 0};
  vertices[3].normal = {0, 1, 0, 0};
  vertices[4].normal = {0, 1, 0, 0};
  vertices[5].normal = {0, 1, 0, 0};

  // fill vertex buffer
  glBufferData(GL_ARRAY_BUFFER, 6 * sizeof(Vertex), vertices, GL_STATIC_DRAW);

  // enable first element as position
  glEnableVertexAttribArray(0);
  glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), 0);

  // enable second element as normal
  glEnableVertexAttribArray(1);
  glVertexAttribPointer(1, 4, GL_FLOAT, GL_TRUE, sizeof(Vertex), (void*)16);

  // unbind buffers
  glBindVertexArray(0);
  glBindBuffer(GL_ARRAY_BUFFER, 0);

  // quad has two triangles
  m_triCount = 2;
}

void Mesh::Initialise(unsigned int vertexCount, const Vertex *vertices,
                      unsigned int indexCount, unsigned int *indices) {
  // check that the mesh is not initialised already
  assert(vao == 0);

  // generate buffers
  glGenBuffers(1, &vbo);
  glGenVertexArrays(1, &vao);

  // bind vortex array -> mesh wrapper
  glBindVertexArray(vao);

  // bind vertex buffer
  glBindBuffer(GL_ARRAY_BUFFER, vbo);

  // fill vertex buffer
  glBufferData(GL_ARRAY_BUFFER, vertexCount * sizeof(Vertex), vertices,
               GL_STATIC_DRAW);

  // enable first element as position
  glEnableVertexAttribArray(0);
  glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), 0);

  // enable second element as normal
  glEnableVertexAttribArray(1);
  glVertexAttribPointer(1, 4, GL_FLOAT, GL_TRUE, sizeof(Vertex), (void *)16);

  // bind indices if there are any
  if (indexCount != 0) {
    glGenBuffers(1, &ibo);

    // bind indice buffer
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);

    // fill buffer
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indexCount * sizeof(unsigned int),
                 indices, GL_STATIC_DRAW);

    m_triCount = indexCount / 3;
  } else {
    m_triCount = vertexCount / 3;
  }

  // unbind buffers
  glBindVertexArray(0);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
  glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void Mesh::InitialiseFromFile(const char *fileName) {
  // Read the vertices from the model
  const aiScene *scene =
      aiImportFile(fileName, 0); // i.e. Blender obj as the file

  // Just use the first mesh we find
  aiMesh *mesh = scene->mMeshes[0];

  // Extract the indices from the first mesh
  int numFaces = mesh->mNumFaces; // A face in this sense is a triangle and
                                  // therefore 3 indices.
  std::vector<unsigned int> indicies;

  for (int i = 0; i < numFaces; i++) {
    // The order matters with mIndices as it determines which way it faces.
    // The renderer draws counter-clockwise.
    indicies.push_back(mesh->mFaces[i].mIndices[0]);
    indicies.push_back(mesh->mFaces[i].mIndices[2]);
    indicies.push_back(mesh->mFaces[i].mIndices[1]);

    // Generate a second triangle for our quads (Blender tends to use quads)
    if (mesh->mFaces[i].mNumIndices == 4) {
      // If it has 4, it is a quad, creating a triangle with the fourth index.
      indicies.push_back(mesh->mFaces[i].mIndices[0]);
      indicies.push_back(mesh->mFaces[i].mIndices[3]);
      indicies.push_back(mesh->mFaces[i].mIndices[2]);
    }
  }

  // Extract vertex data
  int numV = mesh->mNumVertices;
  Vertex *vertices = new Vertex[numV];

  for (int i = 0; i < numV; i++) {
    vertices[i].position = glm::vec4(mesh->mVertices[i].x, mesh->mVertices[i].y,
                                     mesh->mVertices[i].z, 1);
    vertices[i].normal = glm::vec4(mesh->mNormals[i].x, mesh->mNormals[i].y,
                                   mesh->mNormals[i].z, 0);

    // TODO: Normals UVs
  }

  Initialise(numV, vertices, indicies.size(), indicies.data());

  delete[] vertices;
}

void Mesh::ApplyMaterial(aie::ShaderProgram *shader) {
  shader->bindUniform("Ka", Ka);
  shader->bindUniform("Kd", Kd);
  shader->bindUniform("Ks", Ks);
  shader->bindUniform("SpecularPower", specularPower);
}

void Mesh::LoadMaterial(const char *fileName) {
  std::fstream file(fileName, std::ios::in);
  std::string line;
  std::string header;
  char buffer[256];
  while (!file.eof()) {
    // -= Idea (14/02/2024) =-
    // At this point, the values can be modified after reading them from the
    // file and another function could then write the new values back in.

    file.getline(buffer, 256);
    line = buffer;
    std::stringstream ss(line, std::stringstream::in | std::stringstream::out);
    if (line.find("Ka") == 0)
      ss >> header >> Ka.x >> Ka.y >> Ka.z;
    else if (line.find("Ks") == 0)
      ss >> header >> Ks.x >> Ks.y >> Ks.z;
    else if (line.find("Kd") == 0)
      ss >> header >> Kd.x >> Kd.y >> Kd.z;
    else if (line.find("Ns") == 0)
      ss >> header >> specularPower;
  }
}

void Mesh::Draw() {
  glBindVertexArray(vao);

  // Using indices or just vertices
  if (ibo != 0) {
    glDrawElements(GL_TRIANGLES, 3 * m_triCount, GL_UNSIGNED_INT, 0);
  } else {
    glDrawArrays(GL_TRIANGLES, 0, 3 * m_triCount);
  }
}
