#pragma once

#include "tools.h"
#include ENGINE_PLATFORM_INCLUDE_OPENGL


#ifndef INTERNGL_H_PREFIX
#define INTERNGL_H_PREFIX extern
#define INTERNGL_H_POSTFIX
#endif


/* Shaders */
INTERNGL_H_PREFIX PFNGLATTACHSHADERPROC       glAttachShader      INTERNGL_H_POSTFIX;
INTERNGL_H_PREFIX PFNGLBINDATTRIBLOCATIONPROC glBindAttribLocation  INTERNGL_H_POSTFIX;
INTERNGL_H_PREFIX PFNGLCOMPILESHADERPROC      glCompileShader     INTERNGL_H_POSTFIX;
INTERNGL_H_PREFIX PFNGLCREATEPROGRAMPROC      glCreateProgram     INTERNGL_H_POSTFIX;
INTERNGL_H_PREFIX PFNGLCREATESHADERPROC       glCreateShader      INTERNGL_H_POSTFIX;
INTERNGL_H_PREFIX PFNGLDELETEPROGRAMPROC      glDeleteProgram     INTERNGL_H_POSTFIX;
INTERNGL_H_PREFIX PFNGLDELETESHADERPROC       glDeleteShader      INTERNGL_H_POSTFIX;
INTERNGL_H_PREFIX PFNGLDETACHSHADERPROC       glDetachShader      INTERNGL_H_POSTFIX;
INTERNGL_H_PREFIX PFNGLISPROGRAMPROC          glIsProgram         INTERNGL_H_POSTFIX;
INTERNGL_H_PREFIX PFNGLISSHADERPROC           glIsShader          INTERNGL_H_POSTFIX;
INTERNGL_H_PREFIX PFNGLLINKPROGRAMPROC        glLinkProgram       INTERNGL_H_POSTFIX;
INTERNGL_H_PREFIX PFNGLSHADERSOURCEPROC       glShaderSource      INTERNGL_H_POSTFIX;
INTERNGL_H_PREFIX PFNGLUSEPROGRAMPROC         glUseProgram        INTERNGL_H_POSTFIX;
INTERNGL_H_PREFIX PFNGLUNIFORM1FPROC          glUniform1f         INTERNGL_H_POSTFIX;
INTERNGL_H_PREFIX PFNGLUNIFORM1IPROC          glUniform1i         INTERNGL_H_POSTFIX;
INTERNGL_H_PREFIX PFNGLUNIFORM2FPROC          glUniform2f         INTERNGL_H_POSTFIX;
INTERNGL_H_PREFIX PFNGLUNIFORM3FPROC          glUniform3f         INTERNGL_H_POSTFIX;
INTERNGL_H_PREFIX PFNGLUNIFORM4FPROC          glUniform4f         INTERNGL_H_POSTFIX;
INTERNGL_H_PREFIX PFNGLUNIFORM4FVPROC         glUniform4fv        INTERNGL_H_POSTFIX;
INTERNGL_H_PREFIX PFNGLUNIFORMMATRIX4FVPROC   glUniformMatrix4fv  INTERNGL_H_POSTFIX;
INTERNGL_H_PREFIX PFNGLVERTEXATTRIB1FPROC     glVertexAttrib1f    INTERNGL_H_POSTFIX;
INTERNGL_H_PREFIX PFNGLVERTEXATTRIB2FPROC     glVertexAttrib2f    INTERNGL_H_POSTFIX;
INTERNGL_H_PREFIX PFNGLVERTEXATTRIB3FPROC     glVertexAttrib3f    INTERNGL_H_POSTFIX;
INTERNGL_H_PREFIX PFNGLVERTEXATTRIB4FPROC     glVertexAttrib4f    INTERNGL_H_POSTFIX;
INTERNGL_H_PREFIX PFNGLGETOBJECTPARAMETERFVARBPROC  glGetObjectParameterfvARB INTERNGL_H_POSTFIX;
INTERNGL_H_PREFIX PFNGLGETOBJECTPARAMETERIVARBPROC  glGetObjectParameterivARB INTERNGL_H_POSTFIX;
INTERNGL_H_PREFIX PFNGLGETINFOLOGARBPROC      glGetInfoLogARB     INTERNGL_H_POSTFIX;
INTERNGL_H_PREFIX PFNGLGETSHADERIVPROC        glGetShaderiv       INTERNGL_H_POSTFIX;
INTERNGL_H_PREFIX PFNGLGETSHADERINFOLOGPROC   glGetShaderInfoLog  INTERNGL_H_POSTFIX;
INTERNGL_H_PREFIX PFNGLGETUNIFORMLOCATIONPROC glGetUniformLocation            INTERNGL_H_POSTFIX;
INTERNGL_H_PREFIX PFNGLGENBUFFERSPROC         glGenBuffers        INTERNGL_H_POSTFIX;
INTERNGL_H_PREFIX PFNGLDELETEBUFFERSPROC      glDeleteBuffers     INTERNGL_H_POSTFIX;
INTERNGL_H_PREFIX PFNGLBINDBUFFERPROC         glBindBuffer        INTERNGL_H_POSTFIX;
INTERNGL_H_PREFIX PFNGLBUFFERDATAPROC         glBufferData        INTERNGL_H_POSTFIX;
INTERNGL_H_PREFIX PFNGLGENVERTEXARRAYSPROC    glGenVertexArrays   INTERNGL_H_POSTFIX;
INTERNGL_H_PREFIX PFNGLBINDVERTEXARRAYPROC    glBindVertexArray   INTERNGL_H_POSTFIX;
INTERNGL_H_PREFIX PFNGLDELETEVERTEXARRAYSPROC glDeleteVertexArrays  INTERNGL_H_POSTFIX;
INTERNGL_H_PREFIX PFNGLENABLEVERTEXATTRIBARRAYPROC glEnableVertexAttribArray INTERNGL_H_POSTFIX;
INTERNGL_H_PREFIX PFNGLDISABLEVERTEXATTRIBARRAYPROC glDisableVertexAttribArray INTERNGL_H_POSTFIX;
INTERNGL_H_PREFIX PFNGLVERTEXATTRIBPOINTERPROC glVertexAttribPointer INTERNGL_H_POSTFIX;

#if IS_WIN32
INTERNGL_H_PREFIX PFNGLACTIVETEXTUREPROC      glActiveTexture     INTERNGL_H_POSTFIX;
#endif

INTERNGL_H_PREFIX PFNGLGETATTRIBLOCATIONPROC  glGetAttribLocation INTERNGL_H_POSTFIX;


/* WGL_EXT_swap_control */
#if IS_WIN32
INTERNGL_H_PREFIX PFNWGLSWAPINTERVALEXTPROC   wglSwapIntervalEXT  INTERNGL_H_POSTFIX;

/* WGL_EXT_extensions_string */
INTERNGL_H_PREFIX PFNWGLGETEXTENSIONSSTRINGARBPROC wglGetExtensionsStringARB INTERNGL_H_POSTFIX;
#endif

/* Frame buffer object */
INTERNGL_H_PREFIX PFNGLGENFRAMEBUFFERSPROC    glGenFramebuffers   INTERNGL_H_POSTFIX;
INTERNGL_H_PREFIX PFNGLBINDFRAMEBUFFERPROC    glBindFramebuffer   INTERNGL_H_POSTFIX;
INTERNGL_H_PREFIX PFNGLFRAMEBUFFERTEXTUREPROC glFramebufferTexture          INTERNGL_H_POSTFIX;
INTERNGL_H_PREFIX PFNGLFRAMEBUFFERTEXTURE2DPROC   glFramebufferTexture2D    INTERNGL_H_POSTFIX;
INTERNGL_H_PREFIX PFNGLCHECKFRAMEBUFFERSTATUSPROC glCheckFramebufferStatus  INTERNGL_H_POSTFIX;
INTERNGL_H_PREFIX PFNGLDELETEFRAMEBUFFERSPROC glDeleteFramebuffers          INTERNGL_H_POSTFIX;
INTERNGL_H_PREFIX PFNGLDRAWBUFFERSPROC        glDrawBuffers       INTERNGL_H_POSTFIX;

#undef INTERNGL_H_PREFIX
#undef INTERNGL_H_POSTFIX
