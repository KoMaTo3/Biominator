#include "texture.h"
#include "tools.h"
#include "platform/opengl.h"

using namespace Engine;

Texture::Texture( size_t setWidth, size_t setHeight, unsigned char *data, bool setIsTransparent )
  :width( setWidth ), height( setHeight ), isTransparent( setIsTransparent ), textureId( 0 ) {
  if( data ) {
    this->MakeFromBuffer( setWidth, setHeight, data );
  } else {
    LOGE( "Texture => data is null" );
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
  LOGI( "Texture::MakeFromBuffer => size[%dx%d] data[x%p] id[%d] err[x%X]", ( int ) setWidth, ( int ) setHeight, data, ( int ) this->textureId, glGetError() );
}//MakeFromBuffer
