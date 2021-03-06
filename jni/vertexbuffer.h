#pragma once

#include "math/kvector.h"
#include "platform/opengl.h"
//#include "perobjectshaderbuffer.h"
#include "types.h"
#include "math/kmatrix.h"
#include <vector>

namespace Engine {

typedef GLushort VerticeIndex;

struct Vertice {
  Vec3 pos;
  Vec2 tex;
  Vec4 color;

  Mat4 modelMatrix;
  Vec2 texCoordsScale;
  Vec2 texCoordsOffset;
};

class IndicesBlock {
public:
  unsigned short begin;
  unsigned short size;

  IndicesBlock( unsigned short setBegin, unsigned short setSize );
};

typedef std::vector< Vertice > VerticesVector;
typedef std::vector< IndicesBlock > IndicesVector;

class VertexBufferWriter;


class IVertexBufferPool {
public:
  virtual void Delete( VertexBufferWriter* buffer ) = 0;
  virtual ~IVertexBufferPool();
};


class VertexBuffer: public IVertexBufferPool {
public:
  VertexBuffer();
  virtual ~VertexBuffer();
  VertexBufferWriter* New( unsigned short size );
  virtual void Delete( VertexBufferWriter* buffer );
  virtual void Bind();

protected:
  VerticesVector verticesList;
  IndicesVector freeVerticesList;

private:
  VertexBuffer( VertexBuffer& );
  VertexBuffer& operator=( VertexBuffer& );
};


class VertexBufferWriter {
public:
  VertexBufferWriter( unsigned short setBegin, unsigned short setSize, VerticesVector *setVerticesList, IVertexBufferPool* setVertexBuffer );
  virtual ~VertexBufferWriter();
  bool Congregate( IndicesBlock* freeBlock );
  void PushToBuffer( IndicesVector& buffer );
  void Delete();
  Vertice& operator[]( unsigned short index );
  Vertice& Get( unsigned short index );
  KM_INLINE bool IsValid() { return ( this->size > 0 ); }
  KM_INLINE unsigned short GetSize() { return this->size; }
  KM_INLINE unsigned short GetBeginIndex() { return this->begin; }

private:
  VertexBufferWriter();
  VertexBufferWriter( VertexBufferWriter& );
  VertexBufferWriter& operator=( VertexBufferWriter& );

  unsigned short
    begin,
    size;
  VerticesVector *verticesList;
  IVertexBufferPool *vertexBuffer;
};

};
