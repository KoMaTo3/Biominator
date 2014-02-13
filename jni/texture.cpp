#include "texture.h"
#include "tools.h"

using namespace Engine;

Texture::Texture( size_t setWidth, size_t setHeight, unsigned char *data, bool setIsTransparent )
  :textureId( 0 ), width( setWidth ), height( setHeight ), isTransparent( setIsTransparent ) {
  if( data ) {
    this->MakeFromBuffer( setWidth, setHeight, data );
  }
}

Texture::~Texture() {
  if( this->textureId ) {
    LOGI( "~Texture => id[%d]", this->textureId );
    glDeleteTextures( 1, &this->textureId );
  }
}

void Texture::MakeFromBuffer( size_t setWidth, size_t setHeight, unsigned char *data ) {
  glGenTextures( 1, &this->textureId );
  glPixelStorei( GL_UNPACK_ALIGNMENT, 1 );
  glBindTexture( GL_TEXTURE_2D, this->textureId );
  glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST );
  glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST );
  glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT );
  glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT );
  glTexImage2D( GL_TEXTURE_2D, 0, GL_RGBA, setWidth, setHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, data );
  LOGI( "Texture::MakeFromBuffer => size[%dx%d] data[x%p] id[%d] err[x%X]", setWidth, setHeight, data, this->textureId, glGetError() );
}//MakeFromBuffer
