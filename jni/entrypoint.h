#pragma once
#include "core.h"

namespace Engine {

class EntryPoint {
public:
  EntryPoint();
  ~EntryPoint();
  void Run( Core* core );

private:
  EntryPoint( EntryPoint& );
  EntryPoint& operator=( EntryPoint& );
  void Init( Core* core );
  void Destroy();
};

};
