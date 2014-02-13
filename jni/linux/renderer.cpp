//#define GL_GLEXT_PROTOTYPES
#include <vector>
#include <math.h>

#include "../tools.h"

#include "renderer.h"
#include "../eventtypes.h"
#include "opengl.h"

using namespace Engine;

RendererLinuxGL::RendererLinuxGL( Display *setDisplay, XVisualInfo *setVisualInfo, Window *setWindow, const unsigned int setScreenWidth, const unsigned int setScreenHeight )
:Renderer( setScreenWidth, setScreenHeight )
,context( NULL ), display( setDisplay ), visualInfo( setVisualInfo ), window( setWindow ) {
  this->isValid = true;
  this->isValid &= this->InitNativeDisplay();
}

RendererLinuxGL::~RendererLinuxGL() {
  this->DestroyNativeDisplay();
}

void RendererLinuxGL::Render() {
  if( !this->context ) {
    return;
  }
  this->TouchEvent( EVENT_TYPE_RENDERER_BEFORE_RENDER, NULL );
  static float f = 0.0f;
  f += 0.01f;
  glClearColor( sinf( f ) * 0.5f + 0.5f, cosf( f ) * 0.5f + 0.5f, 0.5f, 1.0f );
  glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
  glEnable( GL_DEPTH_TEST );
  glEnable( GL_CULL_FACE );
  glFrontFace( GL_CW );
  glCullFace( GL_BACK );
  this->TouchEvent( EVENT_TYPE_RENDERER_AFTER_RENDER, NULL );
  glXSwapBuffers( this->display, *this->window );
}//Render

bool RendererLinuxGL::InitNativeDisplay() {
  this->context = glXCreateContext( this->display, this->visualInfo, NULL, GL_TRUE );
  glXMakeCurrent( this->display, *this->window, this->context );
  glClearColor( 0.5f, 1.0f, 0.5f, 1.0f );
  this->InitViewport();
  glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
  this->TouchEvent( Engine::EVENT_TYPE_RENDERER_INIT, NULL );
  return true;
}//InitNativeDisplay

void RendererLinuxGL::DestroyNativeDisplay() {
  this->TouchEvent( Engine::EVENT_TYPE_RENDERER_BEFORE_DESTROY, NULL );

  glXMakeCurrent( this->display, 0, 0 );
  glXDestroyContext( this->display, this->context );
  this->context = 0;
  this->display = 0;

  this->TouchEvent( Engine::EVENT_TYPE_RENDERER_AFTER_DESTROY, NULL );
}//DestroyNativeDisplay

void RendererLinuxGL::InitExtensions() {
}//InitExtensions

void RendererLinuxGL::InitViewport() {
  glClearColor( 0.0f, 0.0f, 0.0f, 0.0f );
  glClearDepth( 1.0f );
  glDepthFunc( GL_LEQUAL );
  glEnable( GL_DEPTH_TEST );
  glEnable( GL_TEXTURE_2D );
  glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );
  glEnable( GL_BLEND );
  glPolygonMode( GL_FRONT_AND_BACK, GL_FILL );
  glViewport( 0, 0, this->screenWidth, this->screenHeight );
}//InitViewport
