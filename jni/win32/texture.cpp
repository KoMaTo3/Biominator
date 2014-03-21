#include "texture.h"
#include "../tools.h"

using namespace Engine;

TextureWin32::TextureWin32( size_t setWidth, size_t setHeight, unsigned char *data, bool setIsTransparent, bool setIsCompressed, size_t setDataLength, ImageType setImageFormat )
:Texture( setWidth, setHeight, data, setIsTransparent, setIsCompressed, setDataLength, setImageFormat ) {
  savedBuffer.Alloc( setDataLength );
  memcpy( savedBuffer.GetData(), data, savedBuffer.GetLength() );
}

TextureWin32::~TextureWin32() {
}

void TextureWin32::ReInitialize() {
  this->MakeFromBuffer( this->width, this->height, this->savedBuffer.GetData(), this->dataLength );
}

void TextureWin32::PlaceToAtlas( Texture* textureAtlas, Engine::Rect< uint32_t > *rect ) {
  TextureWin32 *atlas = static_cast< TextureWin32* >( textureAtlas );
  //LOGI( "TextureWin32::PlaceToAtlas => atlas[%p] [%d; %d]-[%d; %d]", textureAtlas, rect->left, rect->top, rect->right, rect->bottom );

  Engine::Rect< uint32_t > rectInAtlas;
  if( rect ) {
    rectInAtlas = *rect;
  } else {
    atlas->BindTextureToThisAtlas( this );
  }

  uint32_t
    destStride = atlas->width * 4,
    srcStride = this->width * 4,
    bytesPerRow = this->width * 4;
  uint8_t
    *srcData = this->savedBuffer.GetData(),
    *destData = atlas->savedBuffer.GetData() + rectInAtlas.top * destStride + ( rectInAtlas.left << 2 );

  LOGI( "src[%p:%d][%d x %d]", this->savedBuffer.GetData(), this->savedBuffer.GetLength(), this->width, this->height );
  LOGI( "dest[%p:%d][%d x %d]", atlas->savedBuffer.GetData(), atlas->savedBuffer.GetLength(), atlas->width, atlas->height );
  for( uint32_t y = 0; y < this->height; ++y ) {
    memcpy( destData, srcData, bytesPerRow );
    srcData += srcStride;
    destData += destStride;
  }

  if( this->textureId ) {
    glDeleteTextures( 1, &this->textureId );
    this->textureId = atlas->textureId;
    this->atlasFotThis = atlas;
  }

  atlas->MakeFromBuffer( atlas->width, atlas->height, atlas->savedBuffer.GetData(), atlas->dataLength );
}//PlaceToAtlas
