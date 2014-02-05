#include "predefines.h"
#include "tools.h"
#include ENGINE_PLATFORM_INCLUDE_OPENGL
#include "vertexbuffergl.h"
#include "renderer.h"
#include "eventtypes.h"

using namespace Engine;

VertexBufferGL::VertexBufferGL( Renderer* setRenderer )
:VertexBuffer(), Listener() {
  glGenBuffers( 1, &this->vbo );
  setRenderer->Listen( this, EVENT_TYPE_RENDERER_BEFORE_RENDER, VertexBufferGL::OnBeforeRender );
}

VertexBufferGL::~VertexBufferGL() {
  glDeleteBuffers( 1, &this->vbo );
  LOGI( "~VertexBufferGL[%p]", this );
}

void VertexBufferGL::OnBeforeRender( Listener* listener, Producer *producer, int eventId, void *data ) {
  VertexBufferGL *isThis = ( VertexBufferGL* ) listener;
  LOGI( "VertexBufferGL::OnBeforeRender => vertices[%d] this[%p]", ( int ) isThis->verticesList.size(), isThis );
  if( isThis->verticesList.empty() ) {
    return;
  }
  glBindBuffer( GL_ARRAY_BUFFER, isThis->vbo );
  glBufferData( GL_ARRAY_BUFFER, isThis->verticesList.size() * sizeof( Vertice ), &isThis->verticesList[ 0 ], GL_STATIC_DRAW );
  LOGI( "VertexBufferGL::OnBeforeRender => vertices committed" );
}//OnBeforeRender
