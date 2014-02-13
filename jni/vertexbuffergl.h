#pragma once

#include "vertexbuffer.h"
#include "listener.h"
#include "platform/opengl.h"

namespace Engine {

class Producer;
class Renderer;

class VertexBufferGL: public VertexBuffer, public Listener {
public:
  VertexBufferGL( Renderer* setRenderer );
  virtual ~VertexBufferGL();
  static void OnBeforeRender( Listener* listener, Producer *producer, int eventId, void *data );
  virtual void Bind();

private:
  VertexBufferGL();
  VertexBufferGL( VertexBufferGL& );
  VertexBufferGL& operator=( VertexBufferGL& );

  GLuint vbo;
};

};
