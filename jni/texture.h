#pragma once

#include "platform/opengl.h"
#include "imageloader.h"
#include "math/kvector.h"
#include "squareatlas.h"

namespace Engine {

class Texture;
typedef Engine::Rect< uint32_t > TexturePlaceRect;

enum TextureFilterType {
  TEXTURE_FILTER_TYPE_POINT = GL_NEAREST,
  TEXTURE_FILTER_TYPE_LINEAR = GL_LINEAR,
};

class Texture {
public:
  Texture( size_t setWidth, size_t setHeight, unsigned char *data, bool setIsTransparent = false, bool setIsCompressed = false, size_t setDataLength = 0, ImageType setImageFormat = IMAGE_TYPE_UNKNOWN, TextureFilterType setFilterType = TEXTURE_FILTER_TYPE_POINT );
  virtual ~Texture();
  virtual void ReInitialize() = 0;
  inline bool IsTransparent() const { return this->isTransparent; };
  inline GLuint GetTextureId() const { return this->textureId; };
  inline Vec2 GetCoordsOffset() const { return this->texCoordsOffset; };
  inline Vec2 GetCoordsScale() const { return this->texCoordsScale; };
  bool InitAtlas();
  bool BindTextureToThisAtlas( Texture* texture );
  bool UnbindTextureFromThisAtlas( Texture* texture );
  inline size_t GetWidth() const { return this->width; };
  inline size_t GetHeight() const { return this->height; };
  inline bool IsAtlas() const { return this->atlas != 0; };

protected:
  virtual void MakeFromBuffer( size_t setWidth, size_t setHeight, unsigned char *data, size_t dataLength );
  virtual void PlaceToAtlas( Texture* textureAtlas, Engine::Rect< uint32_t > *rect );
  virtual void ClearPlaceInAtlas( Texture* textureAtlas );

  size_t
    width,
    height,
    dataLength;
  bool isTransparent;
  bool isCompressed;
  ImageType imageFormat;
  Vec2
    texCoordsOffset,
    texCoordsScale;
  SquareAtlas *atlas;
  Texture *atlasFotThis;
  TexturePlaceRect *placeInAtlas;
  TextureFilterType filterType;

  GLuint textureId;

private:
  Texture();
  Texture( Texture& );
  Texture& operator=( Texture& );
};

};
