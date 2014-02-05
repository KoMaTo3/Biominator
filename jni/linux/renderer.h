#pragma once

//#define GL_GLEXT_PROTOTYPES
#include <GL/gl.h>
#include <GL/glx.h>
#include <GL/glu.h>
#include <GL/glext.h>
#include "../renderer.h"
#include <string>

namespace Engine {

class RendererLinuxGL: public Renderer {
public:
  RendererLinuxGL( Display *setDisplay, XVisualInfo *setVisualInfo, Window *setWindow, const unsigned int setScreenWidth, const unsigned int setScreenHeight );
  virtual ~RendererLinuxGL();
  virtual void Render();
  virtual void InitViewport();

protected:
  virtual void InitExtensions();

private:
  RendererLinuxGL();
  RendererLinuxGL( const RendererLinuxGL& );
  RendererLinuxGL& operator=( const RendererLinuxGL& );
  bool InitNativeDisplay();
  void DestroyNativeDisplay();
  bool LoadExtension( const std::string &name, void** function );

  GLXContext context;
  Display *display;
  XVisualInfo *visualInfo;
  Window *window;
};

};
