#pragma once
#include "mesh.h"
#include "glm/glm.hpp"

class MeshRenderer : public Mesh {

public:
  struct Vertex {
	glm::vec4 position;
	glm::vec4 normal;
	glm::vec4 tex_coord;
  };

  void InitialiseQuad();
  virtual void Draw();
};
