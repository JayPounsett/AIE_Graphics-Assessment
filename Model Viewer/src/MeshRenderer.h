#pragma once
#include "Mesh.h"
#include "glm/glm.hpp"

class MeshRenderer : public Mesh {

public:
  struct Vertex {
	glm::vec4 m_Position;
	glm::vec4 m_Normal;
	glm::vec4 m_TexCoord;
  };

  void InitialiseQuad();
  virtual void Draw();
};
