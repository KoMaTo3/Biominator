#pragma once

#include "windows.h"
#include "opengl/gl.h"

#include "../renderer.h"

namespace Engine {

class RendererWin32gl: public Engine::Renderer {
public:
  RendererWin32gl( HWND setWindow );
  virtual ~RendererWin32gl();
  virtual void Render();

private:
  RendererWin32gl();
  RendererWin32gl( const RendererWin32gl& );
  RendererWin32gl& operator=( const RendererWin32gl& );
  bool InitNativeDisplay();
  void DestroyNativeDisplay();
  bool InitPixelFormat();
  void InitViewport();

  HWND window;
  HGLRC context;
  HPALETTE palette;
  HDC display;
};

};
