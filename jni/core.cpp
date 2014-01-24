#include "core.h"
#include "renderer.h"
#include "tools.h"

#include <stdint.h>

using namespace Engine;

Core::Core()
:Producer(), isValid( true ), renderer( 0 ), isFocused( false ), isVisible( true ) {
}

Core::~Core() {
  SAFE_DELETE( this->renderer );
  LOGE( "Core deleted ok" );
}

void Core::Run() {
  while( this->isValid ) {
    //LOGE("tick");
    this->Update();
    if( this->isValid && this->isVisible && this->isFocused && this->renderer && this->renderer->IsValid() ) {
      this->renderer->Render();
    }
  }
  LOGE("Run done...");
}//Run

const std::string Core::GetPlatform() const {
  return ENGINE_PLATFORM;
}
