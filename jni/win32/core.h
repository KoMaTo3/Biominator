#pragma once

#include "../core.h"

#include <windows.h>

namespace Engine {

class RendererWin32;

class CoreWin32: public Core {
public:
  CoreWin32( HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow );
  virtual ~CoreWin32();
  virtual void Update();
  virtual void Destroy();
  virtual void Stop();
  static LRESULT APIENTRY HandleCmd( HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam );
  HWND GetWindow();

private:
  CoreWin32();
  CoreWin32( const CoreWin32& );
  CoreWin32& operator=( const CoreWin32& );

  HWND windowHandle;
};

};
