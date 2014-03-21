#pragma once

#include "math/kmatrix.h"
#include "math/kvector.h"
#include "platform/opengl.h"
#include <vector>
#include <deque>

namespace Engine {

class PerObjectShaderBuffer {
public:
  typedef GLushort IndexType;

# pragma pack( push, 4 )
  struct Container {
    Mat4 modelMatrix;
    Vec2 TexCoordsScale;
    Vec2 TexCoordsOffset;
  };
# pragma pack( pop )

  PerObjectShaderBuffer();
  virtual ~PerObjectShaderBuffer();
  IndexType AddContainer();
  Container* GetContainer( IndexType index );
  void DeleteContainer( const IndexType index );
  void Update();

private:
  PerObjectShaderBuffer( PerObjectShaderBuffer& );
  PerObjectShaderBuffer& operator=( PerObjectShaderBuffer& );

  typedef std::vector< Container > ContainerList;
  typedef std::deque< IndexType > FreeIndicesList;

  ContainerList items;
  FreeIndicesList freeIndicies;
  GLuint textureId;
};

};
