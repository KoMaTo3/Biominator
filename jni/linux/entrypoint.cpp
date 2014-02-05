#include "entrypoint.h"
#include "core.h"
#include "../tools.h"

FILE *__logFile = NULL;

int main() {
  __logFile = fopen( "log.txt", "wb+" );
  Engine::EntryPointLinux *entryPoint = new Engine::EntryPointLinux();
  Engine::Core *core = new Engine::CoreLinux();
  entryPoint->Run( core );
  SAFE_DELETE( core );
  SAFE_DELETE( entryPoint );
  fclose( __logFile );
  __logFile = NULL;
  return 0;
}

using namespace Engine;

EntryPointLinux::EntryPointLinux()
:EntryPoint() {
}

EntryPointLinux::~EntryPointLinux() {
}
