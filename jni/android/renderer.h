#pragma once

#include "../renderer.h"

#include <EGL/egl.h>
#include <GLES/gl.h>

namespace Engine {

class RendererAndroid: public Engine::Renderer {
public:
  RendererAndroid( ANativeWindow *setWindow );
  virtual ~RendererAndroid();
  virtual void Render();

private:
  RendererAndroid();
  RendererAndroid( const RendererAndroid& );
  RendererAndroid& operator=( const RendererAndroid& );
  void InitVativeDisplay();
  void DestroyNativeDisplay();

  ANativeWindow *window;
  EGLNativeDisplayType display;
  EGLSurface renderSurface;
  EGLContext context;
};

};
