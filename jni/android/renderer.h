#pragma once

#include "../platform/opengl.h"
#include "../renderer.h"

namespace Engine {

class RendererAndroid: public Engine::Renderer {
public:
  RendererAndroid( ANativeWindow *setWindow, const unsigned int setScreenWidth, const unsigned int setScreenHeight );
  virtual ~RendererAndroid();
  virtual void Render();
  virtual void InitViewport();

protected:
  virtual void InitExtensions();

private:
  RendererAndroid();
  RendererAndroid( const RendererAndroid& );
  RendererAndroid& operator=( const RendererAndroid& );
  void InitNativeDisplay();
  void DestroyNativeDisplay();

  ANativeWindow *window;
  EGLNativeDisplayType display;
  EGLSurface renderSurface;
  EGLContext context;
};

};
