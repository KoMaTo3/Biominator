#pragma once

#include "../entrypoint.h"

namespace Engine {

class Core;

class EntryPointAndroid: public EntryPoint {
public:
  EntryPointAndroid();
  ~EntryPointAndroid();

private:
  EntryPointAndroid( EntryPointAndroid& );
  EntryPointAndroid& operator=( EntryPointAndroid& );

};

};
