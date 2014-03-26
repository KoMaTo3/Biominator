#pragma once

#include "predefines.h"

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


struct Size
{
  uint32_t width;
  uint32_t height;

  Size();
  Size( const Size &newSize ) { this->height = newSize.height; this->width = newSize.width; }
  Size( const uint32_t &newWidth, const uint32_t &newHeight );
  Size operator+( const Size& when ){ return Size( this->width + when.width, this->height + when.height ); }
};


template<class T>
struct Rect
{
  T     left,
        top,
        right,
        bottom;

  Rect() {}
  Rect( const Rect<T> &newSize )
    :left( newSize.left ), right( newSize.right ), top( newSize.top ), bottom( newSize.bottom )
  { }
  Rect( T _left, T _top, T _right, T _bottom )
    :left( _left ), top( _top ), right( _right ), bottom( _bottom )
  { }
};

typedef Rect< float > SizeLTRB;

};
