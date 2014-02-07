#include "core.h"
#include "renderer.h"
#include "filemanager.h"
#include "tools.h"

#include <stdint.h>

using namespace Engine;

Core::Core()
:Producer(), renderer( 0 ), isValid( true ), isFocused( false ), isVisible( true )
,animating( false ), screenWidth( 0 ), screenHeight( 0 ), fileManager( 0 ) {
}

Core::~Core() {
  SAFE_DELETE( this->renderer );
  SAFE_DELETE( this->fileManager );
}

void Core::Run() {
  while( this->isValid ) {
    this->Update();
    if( this->animating && this->renderer && this->renderer->IsValid() ) {
      this->renderer->Render();
    }
  }
}//Run

const std::string Core::GetPlatformName() const {
  return ENGINE_PLATFORM_NAME;
}

const PlatformType Core::GetPlatformType() const {
  return ENGINE_PLATFORM_TYPE;
}

FileManager* Core::GetFileManager() {
  return this->fileManager;
}//GetFileManager
