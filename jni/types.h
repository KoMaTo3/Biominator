#pragma once

#include "predefines.h"

namespace Engine {

class Position {
public:
  Position();
  Position( const int setX, const int setY );
  virtual ~Position();

  int
    x,
    y;
};

class Size {
public:
  Size();
  Size( const int setWidth, const int setHeight );
  virtual ~Size();

  int
    width,
    height;
};

};

#if IS_WIN32
#include "win32/types.h"
//WIN32
#elif IS_ANDROID
#include "android/types.h"
//WIN32
#elif IS_LINUX
#include "linux/types.h"
//WIN32
#endif
