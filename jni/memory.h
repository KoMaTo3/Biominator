#pragma once

#include "tools.h"

namespace Engine {

class Memory {
public:
  Memory();
  Memory( size_t setLength );
  Memory( Memory& copyFrom );
  Memory& operator=( Memory& copyFrom );
  virtual ~Memory();
  void Alloc( size_t setLength, bool nullMemory = false );
  void Free();
  void Resize( size_t setLength, bool nullNewMemory = false );
  inline unsigned char* GetData() const { return this->data; }
  inline unsigned char& operator[]( size_t index ) { return this->data[ index ]; };
  inline size_t GetLength() const { return this->length; };

private:

  size_t length;
  unsigned char *data;
};

};
