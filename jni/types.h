#pragma once

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
