#pragma once
#include "predefines.h"
#if IS_LINUX

#include "../texture.h"
#include "../memory.h"
#include <string>

namespace Engine {

//class TextureLinux;
//typedef TextureLinux TextureType;

class TextureLinux: public Texture {
public:
  TextureLinux( size_t setWidth, size_t setHeight, unsigned char *data, bool setIsTransparent = false );
  virtual ~TextureLinux();
  virtual void ReInitialize();

private:
  TextureLinux();
  TextureLinux( TextureLinux& );
  TextureLinux& operator=( TextureLinux& );

  std::string fileName;
};

};

#endif
