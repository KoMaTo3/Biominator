#pragma once

#include "vertexbuffergl.h"

namespace Engine {

class Material;
class Renderer;
class PerObjectShaderBuffer;

class Mesh {
public:
  Mesh( Renderer *setRenderer, Material *setMaterial );
  virtual ~Mesh();
  virtual void Render( float *worldMatrix = 0 );
  virtual void BeforeRender();
  virtual void EnableRender( bool setEnabled = true );
  VertexBufferWriter* ResizeVertexBuffer( unsigned short setSize );
  inline Mesh* SetWorldMatrix( float *setWorldMatrix ) { this->customWorldMatrix = setWorldMatrix; return this; }
  inline Mesh* SetProjectionMatrix( float *setProjectionMatrix ) { this->customProjectionMatrix = setProjectionMatrix; return this; }

  VertexBufferWriter *vertices;

protected:
  float *objectMatrix;
  float *customWorldMatrix;
  float *customProjectionMatrix;
  Material *material;

private:
  Mesh();
  Mesh( Mesh& );
  Mesh& operator=( Mesh& );

  VertexBufferGL vertexBuffer;
  bool enabled;
};

};
