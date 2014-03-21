#include "texture.h"
#include "tools.h"
#include "platform/opengl.h"

using namespace Engine;

Texture::Texture( size_t setWidth, size_t setHeight, unsigned char *data, bool setIsTransparent, bool setIsCompressed, size_t setDataLength, ImageType setImageFormat )
:width( setWidth ), height( setHeight ), isTransparent( setIsTransparent ), textureId( 0 ), isCompressed( setIsCompressed ), dataLength( setDataLength ? setDataLength : setWidth * setHeight * 4 )
,imageFormat( setImageFormat ), texCoordsOffset( Vec2Null ), texCoordsScale( Vec2One ), atlas( 0 ), atlasFotThis( 0 ) {
  if( data ) {
    this->MakeFromBuffer( setWidth, setHeight, data, setDataLength );
  } else {
    LOGE( "Texture => data is null" );
  }
  LOGI( "+Texture %p", this );
}

Texture::~Texture() {
  if( this->textureId && !atlasFotThis ) {
    LOGI( "~Texture => id[%d]", this->textureId );
    glDeleteTextures( 1, &this->textureId );
  }

  SAFE_DELETE( this->atlas );
  LOGI( "~Texture %p", this );
}


bool Texture::InitAtlas() {
  if( this->atlas ) {
    LOGW( "Texture::InitAtlas => already initialized" );
    return true;
  }

  this->atlas = new SquareAtlas();
  atlas->Init( Engine::Size( this->width, this->height ) );
  return true;
}//InitAtlas


bool Texture::BindTextureToThisAtlas( Texture* texture ) {
  if( !texture ) {
    LOGE( "Texture::BindTextureToThisAtlas => texture is null" );
    return false;
  }

  if( !this->atlas ) {
    LOGE( "Texture::BindTextureToThisAtlas => this is not atlas" );
  }

  Engine::Size space( texture->GetWidth() + 2, texture->GetHeight() + 2 );
  if( !this->atlas->HasPlace( space ) ) {
    LOGE( "Texture::BindTextureToThisAtlas => not enough space" );
    return false;
  }

  Engine::Rect< uint32_t > resultPlace;
  this->atlas->Cut( space, &resultPlace );
  texture->PlaceToAtlas( this, &resultPlace );

  return true;
}//BindTextureToThisAtlas


void Texture::PlaceToAtlas( Texture* texture, Engine::Rect< uint32_t > *rect ) {
  LOGE( "Texture::PlaceToAtlas => NYI" );
}//PlaceToAtlas


void Texture::MakeFromBuffer( size_t setWidth, size_t setHeight, unsigned char *data, size_t dataLength ) {
  if( this->atlasFotThis ) {
    this->PlaceToAtlas( this->atlasFotThis, 0 );
  } else {
    glGenTextures( 1, &this->textureId );
    glPixelStorei( GL_UNPACK_ALIGNMENT, 1 );
    glBindTexture( GL_TEXTURE_2D, this->textureId );
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST );
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST );
    glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT );
    glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT );
    if( this->isCompressed ) {
      LOGI( "glCompressedTexImage2D..." );
      GLenum format = 
        ( this->imageFormat == IMAGE_TYPE_DXT1 ? GL_COMPRESSED_RGBA_S3TC_DXT1_EXT :
        ( this->imageFormat == IMAGE_TYPE_DXT3 ? GL_COMPRESSED_RGBA_S3TC_DXT3_EXT :
        ( this->imageFormat == IMAGE_TYPE_DXT5 ? GL_COMPRESSED_RGBA_S3TC_DXT5_EXT :
        ( this->imageFormat == IMAGE_TYPE_ETC1 ? GL_ETC1_RGB8_OES : 0
        )
        )
        )
        );
      glCompressedTexImage2D( GL_TEXTURE_2D, 0, format, setWidth, setHeight, 0, dataLength, data );
    } else {
      glTexImage2D( GL_TEXTURE_2D, 0, GL_RGBA, setWidth, setHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, data );
    }
    LOGI( "Texture::MakeFromBuffer => size[%dx%d] data[x%p] id[%d] err[x%X]", ( int ) setWidth, ( int ) setHeight, data, ( int ) this->textureId, glGetError() );
  }
}//MakeFromBuffer
