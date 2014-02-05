#pragma once

#include "tools.h"
#include "vertexbuffer.h"
#include "listener.h"
#include ENGINE_PLATFORM_INCLUDE_OPENGL

namespace Engine {

class Producer;
class Renderer;

class VertexBufferGL: public VertexBuffer, public Listener {
public:
  VertexBufferGL( Renderer* setRenderer );
  virtual ~VertexBufferGL();
  static void OnBeforeRender( Listener* listener, Producer *producer, int eventId, void *data );

private:
  VertexBufferGL();
  VertexBufferGL( VertexBufferGL& );
  VertexBufferGL& operator=( VertexBufferGL& );

  GLuint vbo;
};

};