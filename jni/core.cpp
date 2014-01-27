#include "core.h"
#include "renderer.h"
#include "tools.h"

#include <stdint.h>

using namespace Engine;

Core::Core()
:Producer(), isValid( true ), renderer( 0 ), isFocused( false ), isVisible( true ), animating( false ) {
}

Core::~Core() {
  SAFE_DELETE( this->renderer );
}

void Core::Run() {
  while( this->isValid ) {
    this->Update();
    if( this->animating && this->renderer && this->renderer->IsValid() ) {
      this->renderer->Render();
    }
  }
}//Run

const std::string Core::GetPlatform() const {
  return ENGINE_PLATFORM;
}
