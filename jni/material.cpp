//���������� �������������� �� ���������� �������� ���� sprintf
#define _CRT_SECURE_NO_DEPRECATE
//#define _CRT_NONSTDC_NO_DEPRECATE

#include "material.h"
#include "tools.h"
#include "shaderprogram.h"
#include "texture.h"
#include "interngl.h"
#include "platform/opengl.h"

using namespace Engine;

char __buffer1024[ 1024 ];

Material::Material( const std::string& setName, ShaderProgram *setShaderProgram )
:shaderProgram( setShaderProgram ), name( setName ) {
}

Material::~Material() {
}

void Material::AddTexture( Texture* newTexture ) {
  if( newTexture ) {
    sprintf( __buffer1024, "texture%d", ( int ) this->texturesList.size() );
    this->texturesList.insert( std::make_pair( __buffer1024, newTexture ) );
  } else {
    LOGE( "Material::AddTexture => texture is NULL" );
  }
}//AddTexture

void Material::AddColor( const Vec4& newColor ) {
  sprintf( __buffer1024, "color%d", ( int ) this->colorsList.size() );
  this->colorsList.insert( std::make_pair( __buffer1024, newColor ) );
}//AddColor

void Material::Apply() {
  if( !this->shaderProgram ) {
    LOGE( "Material::Apply => shader is NULL" );
    return;
  }
  this->shaderProgram->UseProgram();

  size_t num = 0;
  for( auto& texture: this->texturesList ) {
    glActiveTexture( GL_TEXTURE0 + num );
    glBindTexture( GL_TEXTURE_2D, texture.second->GetTextureId() );
    glUniform1i( this->shaderProgram->GetUniformLocation( texture.first ), num );
    ++num;
  }

  for( auto& color: this->colorsList ) {
    glUniform4fv( this->shaderProgram->GetUniformLocation( color.first ), 1, &color.second.x );
  }
}//Apply
