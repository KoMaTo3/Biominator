#pragma once

#include "platform/opengl.h"
#include "imageloader.h"

namespace Engine {

class Texture {
public:
  Texture( size_t setWidth, size_t setHeight, unsigned char *data, bool setIsTransparent = false, bool setIsCompressed = false, size_t setDataLength = 0, ImageType setImageFormat = IMAGE_TYPE_UNKNOWN );
  virtual ~Texture();
  virtual void ReInitialize() = 0;
  inline bool IsTransparent() const { return this->isTransparent; };
  inline GLuint GetTextureId() const { return this->textureId; };

protected:
  void MakeFromBuffer( size_t setWidth, size_t setHeight, unsigned char *data, size_t dataLength );

  size_t
    width,
    height,
    dataLength;
  bool isTransparent;
  bool isCompressed;
  ImageType imageFormat;

private:
  Texture();
  Texture( Texture& );
  Texture& operator=( Texture& );

  GLuint textureId;
};

};
