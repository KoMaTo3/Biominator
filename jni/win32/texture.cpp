#include "texture.h"
#include "../tools.h"

using namespace Engine;

TextureWin32::TextureWin32( size_t setWidth, size_t setHeight, unsigned char *data, bool setIsTransparent )
:Texture( setWidth, setHeight, data, setIsTransparent ) {
  savedBuffer.Alloc( setWidth * setHeight * 4 );
  memcpy( savedBuffer.GetData(), data, savedBuffer.GetLength() );
}

TextureWin32::~TextureWin32() {
}

void TextureWin32::ReInitialize() {
  this->MakeFromBuffer( this->width, this->height, this->savedBuffer.GetData() );
}
