#include "material.h"
#include "tools.h"

using namespace Engine;

char __buffer1024[ 1024 ];

Material::Material( const std::string& setName, ShaderProgram *setShaderProgram )
:name( setName ), shaderProgram( setShaderProgram ) {
}

Material::~Material() {
}

void Material::AddTexture( Texture* newTexture ) {
  if( newTexture ) {
    sprintf_s( __buffer1024, 1024, "texture%d", this->texturesList.size() );
    this->texturesList.insert( std::make_pair( __buffer1024, newTexture ) );
  } else {
    LOGE( "Material::AddTexture => texture is NULL" );
  }
}//AddTexture

void Material::AddColor( const Vec4& newColor ) {
  sprintf_s( __buffer1024, 1024, "color%d", this->colorsList.size() );
  this->colorsList.insert( std::make_pair( __buffer1024, newColor ) );
}//AddColor
