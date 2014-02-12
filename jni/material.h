#pragma once

#include "math/kvector.h"
#include <unordered_map>
#include <string>

namespace Engine {

class Texture;
class ShaderProgram;
typedef std::unordered_map< std::string, Vec4 > MaterialColorsList;
typedef std::unordered_map< std::string, Texture* > MaterialTexturesList;

class Material {
public:
  Material( const std::string& setName, ShaderProgram *setShaderProgram );
  virtual ~Material();
  void AddTexture( Texture* newTexture );
  void AddColor( const Vec4& newColor );
  void Apply();

private:
  Material();
  Material( Material& );
  Material& operator=( Material& );

  std::string name;
  ShaderProgram *shaderProgram;
  MaterialColorsList colorsList;
  MaterialTexturesList texturesList;
};

};
