#pragma once

#include "vertexbuffergl.h"

namespace Engine {

class Material;
class Renderer;

class Mesh {
public:
  Mesh( Renderer *setRenderer, Material *setMaterial );
  virtual ~Mesh();
  virtual void Render();
  VertexBufferWriter *vertices;
  VertexBufferWriter* ResizeVertexBuffer( unsigned short setSize );

private:
  Mesh();
  Mesh( Mesh& );
  Mesh& operator=( Mesh& );

  Material *material;
  VertexBufferGL vertexBuffer;
};

};
