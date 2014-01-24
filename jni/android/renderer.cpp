#include "renderer.h"
#include "math.h"
#include "tools.h"
#include "eventtypes.h"

using namespace Engine;

RendererAndroid::RendererAndroid( ANativeWindow *setWindow, const unsigned int setScreenWidth, const unsigned int setScreenHeight )
:Renderer( setScreenWidth, setScreenHeight ), display( EGL_NO_DISPLAY ), renderSurface( NULL ), context( NULL ), window( setWindow ) {
  this->InitNativeDisplay();
  this->isValid = true;
}

RendererAndroid::~RendererAndroid() {
  this->DestroyNativeDisplay();
}

void RendererAndroid::Render() {
  if( !this->context ) {
    return;
  }
  static float f = 0.0f;
  f += 0.1f;
  glClearColor( sinf( f ) * 0.5f + 0.5f, cosf( f ) * 0.5f + 0.5f, 0.0f, 1.0f );
  glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
  glEnable( GL_DEPTH_TEST );
  glEnable( GL_CULL_FACE );
  glFrontFace( GL_CW );
  glCullFace( GL_BACK );
  eglSwapBuffers( this->display, this->renderSurface );
}//Render

void RendererAndroid::InitNativeDisplay() {
  if( this->display == EGL_NO_DISPLAY )   {
    this->display = eglGetDisplay( EGL_DEFAULT_DISPLAY );
    //assert(this->display);
    EGLBoolean res = eglInitialize( this->display, NULL, NULL );
    //assert(res);
    const EGLint RGBX_8888_ATTRIBS[] =
    {
      EGL_RENDERABLE_TYPE, EGL_OPENGL_ES2_BIT, EGL_SURFACE_TYPE,
      EGL_WINDOW_BIT, EGL_BLUE_SIZE, 8, EGL_GREEN_SIZE, 8,
      EGL_RED_SIZE, 8, EGL_DEPTH_SIZE, 8, EGL_NONE
    };

    const EGLint RGB_565_ATTRIBS[] =
    {
      EGL_RENDERABLE_TYPE, EGL_OPENGL_ES2_BIT, EGL_SURFACE_TYPE,
      EGL_WINDOW_BIT, EGL_BLUE_SIZE, 5, EGL_GREEN_SIZE, 6,
      EGL_RED_SIZE, 5, EGL_DEPTH_SIZE, 8, EGL_NONE
    };

    const EGLint* attribList;

    int windowFormat = ANativeWindow_getFormat( this->window );
    if ((windowFormat == WINDOW_FORMAT_RGBA_8888) || (windowFormat == WINDOW_FORMAT_RGBX_8888)) {
      attribList = RGBX_8888_ATTRIBS;
    } else {
      attribList = RGB_565_ATTRIBS;
    }
    EGLConfig config;
    EGLint numConfigs;
    res = eglChooseConfig(this->display, attribList, &config, 1, &numConfigs);
    //assert(res);

    EGLint format;
    res = eglGetConfigAttrib(this->display, config, EGL_NATIVE_VISUAL_ID, &format);
    //assert(res);
    int32_t setBufRes = ANativeWindow_setBuffersGeometry(this->window, 0, 0, format);
    //assert(setBufRes == 0);

    EGLNativeWindowType windowType;
    this->renderSurface = eglCreateWindowSurface(this->display, config, this->window, NULL);
    //assert(this->renderSurface != EGL_NO_SURFACE);
    EGLint contextAttribs[] = { EGL_CONTEXT_CLIENT_VERSION, 2, EGL_NONE };
    this->context = eglCreateContext(this->display, config, EGL_NO_CONTEXT, contextAttribs);
    //assert(this->context != EGL_NO_CONTEXT);
    res = eglMakeCurrent(this->display, this->renderSurface, this->renderSurface, this->context);
    //assert(res);

    EGLint m_width, m_height;
    res = eglQuerySurface(this->display, this->renderSurface, EGL_WIDTH, &m_width);
    this->screenWidth = m_width;
    //assert(res);
    res = eglQuerySurface(this->display, this->renderSurface, EGL_HEIGHT, &m_height);
    this->screenHeight = m_height;
    //assert(res);

    glClearColor(0.5f, 1.0f, 0.5f, 1.0f);
    this->TouchEvent( Engine::EVENT_TYPE_RENDERER_INIT );

    //m_bInitialized = true;
  } else {
    LOGE( "InitNativeDisplay => already initialized" );
  }
}//InitNativeDisplay

void RendererAndroid::DestroyNativeDisplay() {
  this->TouchEvent( Engine::EVENT_TYPE_RENDERER_BEFORE_DESTROY );
  if (this->display != EGL_NO_DISPLAY) {
    eglMakeCurrent( this->display, EGL_NO_SURFACE, EGL_NO_SURFACE, EGL_NO_CONTEXT );
    if( this->context != EGL_NO_CONTEXT ) {
      eglDestroyContext( this->display, this->context );
    }
    if( this->renderSurface != EGL_NO_SURFACE ) {
      eglDestroySurface( this->display, this->renderSurface );
    }
    eglTerminate( this->display );
  }
  //engine->animating = 0;
  this->display = EGL_NO_DISPLAY;
  this->context = EGL_NO_CONTEXT;
  this->renderSurface = EGL_NO_SURFACE;
  this->isValid = false;
  this->TouchEvent( Engine::EVENT_TYPE_RENDERER_AFTER_DESTROY );
  /*
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
  */
}//DestroyNativeDisplay

void RendererAndroid::InitViewport() {
  //glMatrixMode( GL_PROJECTION );
  //glLoadIdentity();
  //glOrtho( 0.0, 100.0, 100.0, 0.0, 10.0, -100.0 );
  //glMatrixMode( GL_MODELVIEW );
  //glShadeModel( GL_SMOOTH );
  glClearColor( 0.0f, 0.0f, 0.0f, 0.0f );
  //glClearDepth( 1.0f );
  glDepthFunc( GL_LEQUAL );
  //glDepthFunc( GL_NICEST );
  glEnable( GL_DEPTH_TEST );

  glEnable( GL_TEXTURE_2D );

  //glHint( GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST );

  glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );
  glEnable( GL_BLEND );
  //glPolygonMode( GL_FRONT_AND_BACK, GL_FILL );
  glViewport( 0, 0, this->screenWidth, this->screenHeight );
}//InitViewport
