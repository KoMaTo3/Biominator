#include "texture.h"

using namespace Engine;

Texture::Texture( size_t setWidth, size_t setHeight, unsigned char *data )
:textureId( 0 ), width( setWidth ), height( setHeight ) {
  glGenTextures( 1, &this->textureId );
  glPixelStorei( GL_UNPACK_ALIGNMENT, 1 );
  glBindTexture( GL_TEXTURE_2D, this->textureId );
  glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST );
  glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST );
  glTexImage2D( GL_TEXTURE_2D, 0, 4, setWidth, setHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, data );
}

Texture::~Texture() {
  if( this->textureId ) {
    glDeleteTextures( 1, &this->textureId );
  }
}
