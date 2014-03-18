#include "../tools.h"
#include "texture.h"

using namespace Engine;

TextureLinux::TextureLinux( size_t setWidth, size_t setHeight, unsigned char *data, bool setIsTransparent, bool setIsCompressed, size_t setDataLength, ImageType setImageFormat )
:Texture( setWidth, setHeight, data, setIsTransparent, setIsCompressed, setDataLength, setImageFormat ) {
  //this->fileName = "";
}

TextureLinux::~TextureLinux() {
}

void TextureLinux::ReInitialize() {
  //this->MakeFromBuffer( this->width, this->height, this->savedBuffer.GetData(), this->dataLength );
}
