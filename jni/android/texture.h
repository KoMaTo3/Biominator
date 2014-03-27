#pragma once

#include "../texture.h"
#include "../memory.h"
#include <string>

namespace Engine {

class TextureAndroid;
typedef TextureAndroid TextureType;

class TextureAndroid: public Texture {
public:
  TextureAndroid( size_t setWidth, size_t setHeight, unsigned char *data, bool setIsTransparent = false, bool setIsCompressed = false, size_t setDataLength = 0, ImageType setImageFormat = IMAGE_TYPE_UNKNOWN );
  virtual ~TextureAndroid();
  virtual void ReInitialize();

private:
  TextureAndroid();
  TextureAndroid( TextureAndroid& );
  TextureAndroid& operator=( TextureAndroid& );

  std::string fileName;
};

};
