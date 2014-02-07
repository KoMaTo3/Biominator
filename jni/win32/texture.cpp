#include "texture.h"

using namespace Engine;

TextureWin32::TextureWin32( size_t setWidth, size_t setHeight, unsigned char *data )
:Texture( setWidth, setHeight, data ) {
  savedBuffer.Alloc( setWidth * setHeight * 4 );
  memcpy( savedBuffer.GetData(), data, savedBuffer.GetLength() );
}

TextureWin32::~TextureWin32() {
}

void TextureWin32::ReInitialize() {
}
