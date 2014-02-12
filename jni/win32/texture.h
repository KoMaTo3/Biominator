#pragma once

#include "../texture.h"
#include "../memory.h"

namespace Engine {

class TextureWin32;
typedef TextureWin32 TextureType;

class TextureWin32: public Texture {
public:
  TextureWin32( size_t setWidth, size_t setHeight, unsigned char *data, bool setIsTransparent = false );
  virtual ~TextureWin32();
  virtual void ReInitialize();

private:
  TextureWin32();
  TextureWin32( TextureWin32& );
  TextureWin32& operator=( TextureWin32& );

  Memory savedBuffer;
};

};
