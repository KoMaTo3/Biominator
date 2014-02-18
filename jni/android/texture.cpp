#include "android/texture.h"
#include "../tools.h"

using namespace Engine;

TextureAndroid::TextureAndroid( size_t setWidth, size_t setHeight, unsigned char *data, bool setIsTransparent )
:Texture( setWidth, setHeight, data, setIsTransparent ) {
  //this->fileName = "";
}

TextureAndroid::~TextureAndroid() {
}

void TextureAndroid::ReInitialize() {
  //this->MakeFromBuffer( this->width, this->height, this->savedBuffer.GetData() );
}
