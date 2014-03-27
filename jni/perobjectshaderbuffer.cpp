#include "perobjectshaderbuffer.h"
#include "tools.h"
#include "shaderprogram.h"

using namespace Engine;

PerObjectShaderBuffer::PerObjectShaderBuffer() {
  //glGenTextures( 1, &this->textureId );
}

PerObjectShaderBuffer::~PerObjectShaderBuffer() {
  //glDeleteTextures( 1, &this->textureId );
}

PerObjectShaderBuffer::IndexType PerObjectShaderBuffer::AddContainer() {
  if( !this->freeIndicies.empty() ) {
    //PerObjectShaderBuffer::IndexType index = *this->freeIndicies.end();
    this->freeIndicies.pop_back();
  }

  this->items.push_back( Container() );
  size_t newSize = this->items.size();
  if( newSize > 0x0000FFFF ) {
    LOGE( "PerObjectShaderBuffer::AddContainer => index too big" );
    return 0;
  }
  return PerObjectShaderBuffer::IndexType( newSize - 1 );
}//AddContainer

void PerObjectShaderBuffer::DeleteContainer( const PerObjectShaderBuffer::IndexType index ) {
  if( this->items.size() >= index ) {
    LOGE( "PerObjectShaderBuffer::DeleteContainer => index too big [%d]", index );
    return;
  }

  this->freeIndicies.push_back( index );
}//DeleteContainer

void PerObjectShaderBuffer::Update() {
  if( this->items.empty() ) {
    return;
  }
  /*
  glPixelStorei( GL_UNPACK_ALIGNMENT, 1 );
  glBindTexture( GL_TEXTURE_2D, this->textureId );
  glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST );
  glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST );
  glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP );
  glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP );
  glTexImage2D( GL_TEXTURE_2D, 0, GL_RGBA, this->items.size() * sizeof( Container ), 1, 0, GL_RGBA, GL_UNSIGNED_BYTE, &this->items.begin()->modelMatrix[ 0 ][ 0 ] );
  LOGI( "glTexImage1D => %d", this->items.size() * sizeof( Container ) );
  */
}//Update

PerObjectShaderBuffer::Container* PerObjectShaderBuffer::GetContainer( IndexType index ) {
  if( index >= this->items.size() ) {
    LOGE( "PerObjectShaderBuffer::GetContainer => index too big [%d]", index );
    return NULL;
  }

  return &this->items[ index ];
}//GetContainer

void PerObjectShaderBuffer::BindToShader( const ShaderProgram &program, const IndexType index ) {
  if( !this->items.empty() ) {
    LOGI( "vAttrsArray => %d", program.GetUniformLocation( "vAttrsArray" ) );
    LOGI( "PerObjectShaderBuffer::BindToShader => index[%d] address[%p]", index, this->items[ index ].GetPointer() );
    glUniform1fv( program.GetUniformLocation( "vAttrsArray" ), sizeof( Container ) >> 2, this->items[ index ].GetPointer() );
  }
}//BindToShader
