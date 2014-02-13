#include "material.h"
#include "tools.h"
#include "shaderprogram.h"
#include "texture.h"

using namespace Engine;

char __buffer1024[ 1024 ];

Material::Material( const std::string& setName, ShaderProgram *setShaderProgram )
:name( setName ), shaderProgram( setShaderProgram ) {
}

Material::~Material() {
}

void Material::AddTexture( Texture* newTexture ) {
  if( newTexture ) {
    sprintf( __buffer1024, "texture%d", this->texturesList.size() );
    this->texturesList.insert( std::make_pair( __buffer1024, newTexture ) );
  } else {
    LOGE( "Material::AddTexture => texture is NULL" );
  }
}//AddTexture

void Material::AddColor( const Vec4& newColor ) {
  sprintf( __buffer1024, "color%d", this->colorsList.size() );
  this->colorsList.insert( std::make_pair( __buffer1024, newColor ) );
}//AddColor

void Material::Apply() {
  this->shaderProgram->UseProgram();

  int num = 0;
  for( auto& texture: this->texturesList ) {
    LOGI( "Material::Apply => texture['%s'] uniform[%d] id[%d]", texture.first.c_str(), this->shaderProgram->GetUniformLocation( texture.first ), texture.second->GetTextureId() );
    glActiveTexture( GL_TEXTURE0 + num );
    glBindTexture( GL_TEXTURE_2D, texture.second->GetTextureId() );
    glUniform1i( this->shaderProgram->GetUniformLocation( texture.first ), num );
    ++num;
  }

  for( auto& color: this->colorsList ) {
    LOGI( "Material::Apply => name['%s'] uniform[%d] color[%3.1f, %3.1f, %3.1f, %3.1f]", color.first.c_str(), this->shaderProgram->GetUniformLocation( color.first ), color.second.x, color.second.y, color.second.z, color.second.w );
    glUniform4fv( this->shaderProgram->GetUniformLocation( color.first ), 1, &color.second.x );
  }
}//Apply
