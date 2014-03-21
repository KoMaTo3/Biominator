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
  auto
    iterTex = this->texturesList.begin(),
    iterTexEnd = this->texturesList.end();
  while( iterTex != iterTexEnd ) {
    delete *iterTex;
    ++iterTex;
  }

  auto
    iterColor = this->colorsList.begin(),
    iterColorEnd = this->colorsList.end();
  while( iterColor != iterColorEnd ) {
    delete *iterColor;
    ++iterColor;
  }
}

Material* Material::AddTexture( Texture* newTexture ) {
  if( newTexture ) {
    sprintf( __buffer1024, "texture%d", ( int ) this->texturesList.size() );
    MaterialHashContainerTexture *container = new MaterialHashContainerTexture();
    container->name = __buffer1024;
    container->value = newTexture;
    this->texturesList.push_back( container );
  } else {
    LOGE( "Material::AddTexture => texture is NULL" );
  }

  return this;
}//AddTexture

Material* Material::AddColor( const Vec4& newColor ) {
  sprintf( __buffer1024, "color%d", ( int ) this->colorsList.size() );
  //this->colorsList.insert( std::make_pair( __buffer1024, newColor ) );
  MaterialHashContainerColor *container = new MaterialHashContainerColor();
  container->name = __buffer1024;
  container->value = newColor;
  this->colorsList.push_back( container );

  return this;
}//AddColor

void Material::Apply() {
  if( !this->shaderProgram ) {
    LOGE( "Material::Apply => shader is NULL" );
    return;
  }
  LOGI( "apply shader..." );
  this->shaderProgram->UseProgram();

  GLint num = 0;
  LOGI( "apply textures..." );
  for( auto& texture: this->texturesList ) {
    glActiveTexture( GL_TEXTURE0 + num );
    LOGI( "textureId[%d]", texture->value->GetTextureId() );
    glBindTexture( GL_TEXTURE_2D, texture->value->GetTextureId() );
    LOGI( "uniform..." );
    LOGI( "uniform location %d", this->shaderProgram->GetUniformLocation( texture->name ) );
    glUniform1i( this->shaderProgram->GetUniformLocation( texture->name ), num );
    ++num;
  }

  LOGI( "apply colors..." );
  for( auto& color: this->colorsList ) {
    glUniform4fv( this->shaderProgram->GetUniformLocation( color->name ), 1, &color->value.x );
  }
}//Apply

Vec2 Material::GetTextureCoordsOffset( const int layer ) const {
  if( this->texturesList.empty() ) {
    return Vec2Null;
  }

  return this->texturesList[ layer ]->value->GetCoordsOffset();
}//GetTextureCoordsOffset

Vec2 Material::GetTextureCoordsScale( const int layer ) const {
  if( this->texturesList.empty() ) {
    return Vec2Null;
  }

  return this->texturesList[ layer ]->value->GetCoordsScale();
}//GetTextureCoordsScale
