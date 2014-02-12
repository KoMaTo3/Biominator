#pragma once

#include "tools.h"
#include ENGINE_PLATFORM_INCLUDE_OPENGL

namespace Engine {

class Texture {
public:
  Texture( size_t setWidth, size_t setHeight, unsigned char *data, bool setIsTransparent = false );
  virtual ~Texture();
  virtual void ReInitialize() = 0;
  inline bool IsTransparent() const { return this->isTransparent; };

protected:
  void MakeFromBuffer( size_t setWidth, size_t setHeight, unsigned char *data );

  size_t
    width,
    height;
  bool isTransparent;

private:
  Texture();
  Texture( Texture& );
  Texture& operator=( Texture& );

  GLuint textureId;
};

};
