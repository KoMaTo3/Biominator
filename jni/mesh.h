#pragma once

#include "vertexbuffergl.h"

namespace Engine {

class Material;
class Renderer;

class Mesh {
public:
  Mesh( Renderer *setRenderer );
  virtual ~Mesh();

  VertexBufferGL vertexBuffer;

private:
  Mesh();
  Mesh( Mesh& );
  Mesh& operator=( Mesh& );

  Material *material;
};

};
