#pragma once

#include "../entrypoint.h"

namespace Engine {

class Core;

class EntryPointLinux: public EntryPoint {
public:
  EntryPointLinux();
  ~EntryPointLinux();

private:
  EntryPointLinux( EntryPointLinux& );
  EntryPointLinux& operator=( EntryPointLinux& );

};

};
