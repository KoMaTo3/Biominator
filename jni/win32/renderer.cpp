#include "renderer.h"
#include "../tools.h"
#include "../eventtypes.h"

#include <vector>
#include <math.h>

#pragma comment( lib, "opengl32.lib" )

using namespace Engine;

RendererWin32gl::RendererWin32gl( const HWND setWindow, const unsigned int setScreenWidth, const unsigned int setScreenHeight )
:Renderer( setScreenWidth, setScreenHeight ), context( NULL ), display( NULL ), window( setWindow ) {
  this->isValid = true;
  this->isValid &= this->InitNativeDisplay();
}

RendererWin32gl::~RendererWin32gl() {
  this->DestroyNativeDisplay();
}

void RendererWin32gl::Render() {
  if( !this->context ) {
    return;
  }
  static float f = 0.0f;
  f += 0.01f;
  glClearColor( sinf( f ) * 0.5f + 0.5f, cosf( f ) * 0.5f + 0.5f, 0.5f, 1.0f );
  glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
  glEnable( GL_DEPTH_TEST );
  glEnable( GL_CULL_FACE );
  glFrontFace( GL_CW );
  glCullFace( GL_BACK );
  SwapBuffers( this->display );
}//Render

bool RendererWin32gl::InitNativeDisplay() {
  if( !( this->display = GetDC( this->window ) ) ) {
    LOGE( "Core::_InitGraphics => GetDC failed" );
    return false;
  }
  if( !this->InitPixelFormat() ) {
    return false;
  }

  if( !( this->context = wglCreateContext( this->display ) ) ) {
    LOGE( "Core::_InitGraphics => wglCreateContext failed" );
    return false;
  }

  wglMakeCurrent( this->display, this->context );
  this->InitViewport();
  //this->InitExtensions();

  glClearColor( 0.5f, 1.0f, 0.5f, 1.0f );
  this->TouchEvent( Engine::EVENT_TYPE_RENDERER_INIT );
  return true;
}//InitNativeDisplay

void RendererWin32gl::DestroyNativeDisplay() {
  this->TouchEvent( Engine::EVENT_TYPE_RENDERER_BEFORE_DESTROY );
  wglMakeCurrent( NULL, NULL );
  if( this->context ) {
    wglDeleteContext( this->context );
    this->context = NULL;
  }
  if( this->display ) {
    ::ReleaseDC( this->window, this->display );
    this->display = NULL;
  }
  this->TouchEvent( Engine::EVENT_TYPE_RENDERER_AFTER_DESTROY );
}//DestroyNativeDisplay

bool RendererWin32gl::InitPixelFormat() {
  PIXELFORMATDESCRIPTOR pfd =
  {
    sizeof( PIXELFORMATDESCRIPTOR ),/* size */
    1,                              /* version */
    PFD_SUPPORT_OPENGL |
    PFD_DRAW_TO_WINDOW |
    PFD_DOUBLEBUFFER,               /* support double-buffering */
    PFD_TYPE_RGBA,                  /* color type */
    32,                             /* prefered color depth */
    0, 0, 0, 0, 0, 0,               /* color bits (ignored) */
    0,                              /* no alpha buffer */
    0,                              /* alpha bits (ignored) */
    0,                              /* no accumulation buffer */
    0, 0, 0, 0,                     /* accum bits (ignored) */
      32,                             /* depth buffer */
    0,                              /* no stencil buffer */
    0,                              /* no auxiliary buffers */
    PFD_MAIN_PLANE,                 /* main layer */
    0,                              /* reserved */
    0, 0, 0,                        /* no layer, visible, damage masks */
  };
  bool pixelFormatOk = false;
  std::vector<BYTE> depths( 4 );
  depths[ 0 ] = 24;
  depths[ 1 ] = 16;
  depths[ 2 ] = 8;
  depths[ 3 ] = 32;
  for( BYTE num = 0; num < depths.size(); ++num )
  {
    pfd.cDepthBits = depths[ num ];
    int pixelFormat = ChoosePixelFormat( this->display, &pfd );
    if ( pixelFormat ) {
      pixelFormatOk = true;
    } else {
      continue;
    }

    if( ::SetPixelFormat( this->display, pixelFormat, &pfd ) == TRUE ) {
      pixelFormatOk = true;
      break;
    }
  }
  if( !pixelFormatOk ) {
    exit( 1 );
    return false;
  }

  return true;
}//InitPixelFormat

void RendererWin32gl::InitViewport() {
  //glMatrixMode( GL_PROJECTION );
  //glLoadIdentity();
  //glOrtho( 0.0, 100.0, 100.0, 0.0, 10.0, -100.0 );
  //glMatrixMode( GL_MODELVIEW );
  //glShadeModel( GL_SMOOTH );
  glClearColor( 0.0f, 0.0f, 0.0f, 0.0f );
  glClearDepth( 1.0f );
  glDepthFunc( GL_LEQUAL );
  //glDepthFunc( GL_NICEST );
  glEnable( GL_DEPTH_TEST );

  glEnable( GL_TEXTURE_2D );

  //glHint( GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST );

  glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );
  glEnable( GL_BLEND );
  glPolygonMode( GL_FRONT_AND_BACK, GL_FILL );
  glViewport( 0, 0, this->screenWidth, this->screenHeight );
}//InitViewport