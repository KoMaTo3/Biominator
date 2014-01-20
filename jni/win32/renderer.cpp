#include "renderer.h"
#include "../tools.h"

#include <vector>

#pragma comment( lib, "opengl32.lib" )

using namespace Engine;

RendererWin32gl::RendererWin32gl( HWND setWindow )
:Renderer(), context( NULL ), palette( NULL ), display( NULL ), window( setWindow ) {
  this->isValid = true;
  this->isValid &= this->InitNativeDisplay();
}

RendererWin32gl::~RendererWin32gl() {
  wglMakeCurrent( NULL, NULL );
  if( this->context ) {
    wglDeleteContext( this->context );
    this->context = NULL;
  }
  if( this->palette ) {
    ::DeleteObject( this->palette );
  }
  if( this->display ) {
    ::ReleaseDC( this->window, this->display );
    this->display = NULL;
  }
}

void RendererWin32gl::Render() {
  if( !this->context ) {
    return;
  }
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

  glClearColor(0.5f, 1.0f, 0.5f, 1.0f);
  return true;
}//InitNativeDisplay

void RendererWin32gl::DestroyNativeDisplay() {
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
}//InitViewport
