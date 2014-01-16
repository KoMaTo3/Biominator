#include "core.h"
#include "renderer.h"
#include "tools.h"

#include <stdint.h>

using namespace Engine;

Core::Core()
:isValid( true ), renderer( NULL ) {
  SAFE_DELETE( this->renderer );
}

Core::~Core() {
}

void Core::Run() {
  while( this->isValid ) {
    this->Update();
    if( this->renderer ) {
      this->renderer->Render();
    }
  }
}//Run
