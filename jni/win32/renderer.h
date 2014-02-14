#pragma once

#include "../renderer.h"

namespace Engine {

class RendererWin32gl: public Renderer {
public:
  RendererWin32gl( const HWND setWindow, const unsigned int setScreenWidth, const unsigned int setScreenHeight );
  virtual ~RendererWin32gl();
  virtual void Render();
  virtual void InitViewport();

protected:
  virtual void InitExtensions();

private:
  RendererWin32gl();
  RendererWin32gl( const RendererWin32gl& );
  RendererWin32gl& operator=( const RendererWin32gl& );
  bool InitNativeDisplay();
  void DestroyNativeDisplay();
  bool InitPixelFormat();
  bool LoadExtension( const std::string &name, void** function );

  HWND window;
  HGLRC context;
  HDC display;
};

};
