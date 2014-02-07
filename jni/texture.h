#pragma once

#include "tools.h"
#include ENGINE_PLATFORM_INCLUDE_OPENGL

namespace Engine {

class Texture {
public:
  Texture( size_t setWidth, size_t setHeight, unsigned char *data );
  virtual ~Texture();
  virtual void ReInitialize() = 0;

private:
  Texture();
  Texture( Texture& );
  Texture& operator=( Texture& );

  GLuint textureId;
  size_t
    width,
    height;
};

};
