#pragma once

#include "tools.h"
#include "math/kvector.h"
#include <unordered_map>
#include <string>
#include <vector>

namespace Engine {

class Texture;
class ShaderProgram;

class Material {
public:
  Material( const std::string& setName, ShaderProgram *setShaderProgram );
  virtual ~Material();
  Material* AddTexture( Texture* newTexture );
  Material* AddColor( const Vec4& newColor );
  void Apply();
  Vec2 GetTextureCoordsOffset( const int layer = 0 ) const;
  Vec2 GetTextureCoordsScale( const int layer = 0 ) const;

  ShaderProgram *shaderProgram;

private:
  Material();
  Material( Material& );
  Material& operator=( Material& );

  template< class T >
  struct MaterialHashContainer {
    std::string name;
    T value;
  };
  typedef MaterialHashContainer< Texture* > MaterialHashContainerTexture;
  typedef MaterialHashContainer< Vec4 > MaterialHashContainerColor;
  typedef std::vector< MaterialHashContainerTexture* > MaterialTexturesList;
  typedef std::vector< MaterialHashContainerColor* > MaterialColorsList;

  std::string name;
  MaterialColorsList colorsList;
  MaterialTexturesList texturesList;
};

};
