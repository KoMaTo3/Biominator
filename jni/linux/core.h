#pragma once

#include "../core.h"

//#include <X11/X.h>
#include <X11/Xlib.h>
#include<GL/gl.h>
#include<GL/glx.h>
#include<GL/glu.h>

namespace Engine {

class RendererLinux;

class CoreLinux: public Core {
public:
  CoreLinux();
  virtual ~CoreLinux();
  virtual void Update();
  virtual void Destroy();
  virtual void Stop();
  void HandleCmd( XEvent *event );
  //HWND GetWindow();

private:
  CoreLinux( const CoreLinux& );
  CoreLinux& operator=( const CoreLinux& );

  Display *display;
  Window windowRoot;
  Window window;
  const GLint *windowAttr;
  XVisualInfo *visualInfo;
  Colormap colormap;
  XSetWindowAttributes setWndAttr;
  XWindowAttributes getWndAttr;
};

};
