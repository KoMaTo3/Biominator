#pragma once

#include "../predefines.h"

#if IS_WIN32

#include <windows.h>
#include "../win32/opengl/gl.h"
#include "../win32/opengl/glext.h"
#include "../win32/opengl/wglext.h"
#include "../interngl.h"
#define GL_ETC1_RGB8_OES (0)

#elif IS_ANDROID

#include <EGL/egl.h>
#include <GLES2/gl2.h>

#elif IS_LINUX

#define GL_GLEXT_PROTOTYPES
#include<X11/X.h>
#include<X11/Xlib.h>
#include <GL/gl.h>
#include <GL/glx.h>
#include <GL/glu.h>
#include <GL/glext.h>
//#include "../interngl.h"
#define GL_ETC1_RGB8_OES (0)

#endif
