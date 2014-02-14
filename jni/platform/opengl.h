#pragma once

#include "predefines.h"

#if IS_WIN32

#include <windows.h>
#include "../win32/opengl/gl.h"
#include "../win32/opengl/glext.h"
#include "../win32/opengl/wglext.h"

#elif IS_ANDROID

#include <EGL/egl.h>
#include <GLES2/gl2.h>

#elif IS_LINUX

#include <GL/gl.h>
#include <GL/glx.h>
#include <GL/glu.h>
#include <GL/glext.h>
#define GL_GLEXT_PROTOTYPES
#include "../interngl.h"

#endif
