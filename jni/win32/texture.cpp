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
