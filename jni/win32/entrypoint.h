#pragma once

#include "../entrypoint.h"

namespace Engine {

class Core;

class EntryPointWin32: public EntryPoint {
public:
  EntryPointWin32();
  ~EntryPointWin32();

private:
  EntryPointWin32( EntryPointWin32& );
  EntryPointWin32& operator=( EntryPointWin32& );

};

};
