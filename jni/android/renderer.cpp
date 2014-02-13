#include "math.h"

#include "../tools.h"

#include "renderer.h"
#include "../eventtypes.h"

using namespace Engine;

RendererAndroid::RendererAndroid( ANativeWindow *setWindow, const unsigned int setScreenWidth, const unsigned int setScreenHeight )
:Renderer( setScreenWidth, setScreenHeight ), display( EGL_NO_DISPLAY ), renderSurface( EGL_NO_SURFACE )
,context( EGL_NO_CONTEXT ), window( setWindow ) {
  LOGI( "+RendererAndroid" );
  this->InitNativeDisplay();
}

RendererAndroid::~RendererAndroid() {
  LOGI( "~RendererAndroid..." );
  this->DestroyNativeDisplay();
  LOGI( "~RendererAndroid done" );
}

void RendererAndroid::Render() {
  if( !this->isValid ) {
    return;
  }
  this->TouchEvent( EVENT_TYPE_RENDERER_BEFORE_RENDER, NULL );
  static float f = 0.0f;
  f += 0.1f;
  glClearColor( sinf( f ) * 0.5f + 0.5f, cosf( f ) * 0.5f + 0.5f, 0.0f, 1.0f );
  glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
  glEnable( GL_DEPTH_TEST );
  glEnable( GL_CULL_FACE );
  glFrontFace( GL_CW );
  glCullFace( GL_BACK );
  this->TouchEvent( EVENT_TYPE_RENDERER_AFTER_RENDER, NULL );
  eglSwapBuffers( this->display, this->renderSurface );
} //Render

void RendererAndroid::InitNativeDisplay() {
  if( this->isValid ) {
    return;
  }
  if( this->display == EGL_NO_DISPLAY ) {
    this->display = eglGetDisplay( EGL_DEFAULT_DISPLAY );
    EGLBoolean res = eglInitialize( this->display, NULL, NULL );
    const EGLint RGBX_8888_ATTRIBS[] = { EGL_RENDERABLE_TYPE,
        EGL_OPENGL_ES2_BIT, EGL_SURFACE_TYPE, EGL_WINDOW_BIT, EGL_BLUE_SIZE, 8,
        EGL_GREEN_SIZE, 8, EGL_RED_SIZE, 8, EGL_DEPTH_SIZE, 8, EGL_NONE };

    const EGLint RGB_565_ATTRIBS[] = { EGL_RENDERABLE_TYPE, EGL_OPENGL_ES2_BIT,
        EGL_SURFACE_TYPE, EGL_WINDOW_BIT, EGL_BLUE_SIZE, 5, EGL_GREEN_SIZE, 6,
        EGL_RED_SIZE, 5, EGL_DEPTH_SIZE, 8, EGL_NONE };

    const EGLint* attribList;

    int windowFormat = ANativeWindow_getFormat( this->window );
    if( ( windowFormat == WINDOW_FORMAT_RGBA_8888 )
        || ( windowFormat == WINDOW_FORMAT_RGBX_8888 ) ) {
      attribList = RGBX_8888_ATTRIBS;
    } else {
      attribList = RGB_565_ATTRIBS;
    }
    EGLConfig config;
    EGLint numConfigs;
    res = eglChooseConfig( this->display, attribList, &config, 1, &numConfigs );

    EGLint format;
    res = eglGetConfigAttrib( this->display, config, EGL_NATIVE_VISUAL_ID, &format );
    int32_t setBufRes = ANativeWindow_setBuffersGeometry( this->window, 0, 0, format );

    EGLNativeWindowType windowType;
    this->renderSurface = eglCreateWindowSurface( this->display, config, this->window, NULL );
    EGLint contextAttribs[] = { EGL_CONTEXT_CLIENT_VERSION, 2, EGL_NONE };
    this->context = eglCreateContext( this->display, config, EGL_NO_CONTEXT,
        contextAttribs );
    res = eglMakeCurrent( this->display, this->renderSurface, this->renderSurface, this->context );

    EGLint m_width, m_height;
    res = eglQuerySurface( this->display, this->renderSurface, EGL_WIDTH, &m_width );
    this->screenWidth = m_width;
    res = eglQuerySurface( this->display, this->renderSurface, EGL_HEIGHT, &m_height );
    this->screenHeight = m_height;

    glClearColor( 0.5f, 1.0f, 0.5f, 1.0f );
    this->TouchEvent( Engine::EVENT_TYPE_RENDERER_INIT, NULL );

    this->isValid = true;
  } else {
    LOGE( "InitNativeDisplay => already initialized" );
  }
} //InitNativeDisplay

void RendererAndroid::DestroyNativeDisplay() {
  this->TouchEvent( Engine::EVENT_TYPE_RENDERER_BEFORE_DESTROY, NULL );
  if( this->display != EGL_NO_DISPLAY ) {
    eglMakeCurrent( this->display, EGL_NO_SURFACE, EGL_NO_SURFACE, EGL_NO_CONTEXT );
    if( this->context != EGL_NO_CONTEXT ) {
      eglDestroyContext( this->display, this->context );
    }
    if( this->renderSurface != EGL_NO_SURFACE ) {
      eglDestroySurface( this->display, this->renderSurface );
    }
    eglTerminate( this->display );
  }
  this->display = EGL_NO_DISPLAY;
  this->context = EGL_NO_CONTEXT;
  this->renderSurface = EGL_NO_SURFACE;
  this->isValid = false;
  this->TouchEvent( Engine::EVENT_TYPE_RENDERER_AFTER_DESTROY, NULL );
} //DestroyNativeDisplay

void RendererAndroid::InitViewport() {
  if( !this->isValid ) {
    return;
  }
  glClearColor( 0.0f, 0.0f, 0.0f, 0.0f );
  glDepthFunc( GL_LEQUAL );
  glEnable( GL_DEPTH_TEST );
  glEnable( GL_TEXTURE_2D );
  glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );
  glEnable( GL_BLEND );
  glViewport( 0, 0, this->screenWidth, this->screenHeight );
} //InitViewport

void RendererAndroid::InitExtensions() {
}//InitExtensions
