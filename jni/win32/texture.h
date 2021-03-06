#pragma once

#include "../texture.h"
#include "../memory.h"

namespace Engine {

class Texture;
class TextureWin32;
typedef TextureWin32 TextureType;

class TextureWin32: public Texture {
public:
  TextureWin32( size_t setWidth, size_t setHeight, unsigned char *data, bool setIsTransparent = false, bool setIsCompressed = false, size_t setDataLength = 0, ImageType setImageFormat = IMAGE_TYPE_UNKNOWN, TextureFilterType setFilterType = TEXTURE_FILTER_TYPE_POINT );
  virtual ~TextureWin32();
  virtual void ReInitialize();

protected:
  virtual void PlaceToAtlas( Texture* textureAtlas, Engine::Rect< uint32_t > *rect );
  virtual void ClearPlaceInAtlas( Texture* textureAtlas ) override;

private:
  TextureWin32();
  TextureWin32( TextureWin32& );
  TextureWin32& operator=( TextureWin32& );

  Memory savedBuffer;
};

};
