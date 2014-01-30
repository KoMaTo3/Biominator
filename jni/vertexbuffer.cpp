#include "vertexbuffer.h"
#include "math/kmath.h"
#include "tools.h"

using namespace Engine;

VertexBuffer::VertexBuffer() {
}

VertexBuffer::~VertexBuffer() {
}

VertexBufferWriter* VertexBuffer::New( unsigned short size ) {
  if( !size ) {
    return NULL;
  }

  IndicesVector::iterator
    iter = this->freeIndicesList.begin(),
    iterEnd = this->freeIndicesList.end();
  bool finded = false;
  unsigned short begin;

  while( iter != iterEnd ) {
    if( iter->size >= size ) {
      finded = true;
      begin = iter->begin;
      if( iter->size == size ) {
        LOGI( "VertexBuffer::New => delete free index[%d; %d]", iter->begin, iter->size );
        this->freeIndicesList.erase( iter );
      } else {
        LOGI( "VertexBuffer::New => resize free index[%d; %d] to [%d; %d]", iter->begin, iter->size, iter->begin + size, iter->size - size );
        iter->begin += size;
        iter->size -= size;
      }
      break;
    }
    ++iter;
  }//while

  if( !finded ) {
    this->verticesList.resize( this->verticesList.size() + size );
    begin = this->verticesList.size() - size;
    LOGI( "VertexBuffer::New => alloc new vertices, new size = %d", this->verticesList.size() );
  }

  VertexBufferWriter *buffer = new VertexBufferWriter( begin, size, &this->verticesList, this );

  return buffer;
}//New

void VertexBuffer::Delete( VertexBufferWriter* buffer ) {
  if( !buffer || !buffer->IsValid() ) {
    LOGE( "VertexBuffer::Delete => buffer is not valid" );
    return;
  }

  bool finded = false;
  for( auto& iter: this->freeIndicesList ) {
    if( buffer->Congregate( &iter ) ) {
      finded = true;
      break;
    }
  }

  if( !finded ) {
    buffer->PushToBuffer( this->freeIndicesList );
  }

  delete buffer;
}//Delete

void VertexBufferWriter::PushToBuffer( IndicesVector& buffer ) {
  buffer.push_back( IndicesBlock( this->begin, this->size ) );
  LOGI( "VertexBufferWriter::PushToBuffer => new free index[%d; %d]", this->begin, this->size );
  this->size = 0;
}//PushToBuffer

VertexBufferWriter::VertexBufferWriter( unsigned short setBegin, unsigned short setSize, VerticesVector *setVerticesList, IVertexBufferPool* setVertexBuffer )
:begin( setBegin ), size( setSize ), verticesList( setVerticesList ), vertexBuffer( setVertexBuffer ) {
  LOGI( "+VertexBufferWriter => begin[%d] size[%d]", this->begin, this->size );
}

VertexBufferWriter::~VertexBufferWriter() {
}

void VertexBufferWriter::Delete() {
  if( this->IsValid() ) {
    this->vertexBuffer->Delete( this );
  }
}//Delete

bool VertexBufferWriter::Congregate( IndicesBlock* freeBlock ) {
  if( freeBlock->begin == this->begin + this->size ) {
    LOGI( "VertexBufferWriter::Congregate => with index[%d; %d] to [%d; %d]", freeBlock->begin, freeBlock->size, freeBlock->begin - this->size, freeBlock->size + this->size );
    freeBlock->begin -= this->size;
    freeBlock->size += this->size;
    this->size = 0;
    return true;
  } else if( freeBlock->begin + freeBlock->size == this->begin ) {
    LOGI( "VertexBufferWriter::Congregate => with index[%d; %d] to [%d; %d]", freeBlock->begin, freeBlock->size, freeBlock->begin, freeBlock->size + this->size );
    freeBlock->size += this->size;
    this->size = 0;
    return true;
  }
  LOGI( "VertexBufferWriter::Congregate => can't" );
  return false;
}//Congregate


IndicesBlock::IndicesBlock( unsigned short setBegin, unsigned short setSize )
:begin( setBegin ), size( setSize ) {
}

Vertice& VertexBufferWriter::operator[]( unsigned short index ) {
  return ( *this->verticesList )[ this->begin + Min2( index, ( unsigned short ) ( this->size - 1 ) ) ];
}

Vertice& VertexBufferWriter::Get( unsigned short index ) {
  return this->operator[]( index );
}
