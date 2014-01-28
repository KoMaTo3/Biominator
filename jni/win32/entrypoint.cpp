#include "entrypoint.h"
#include "core.h"
#include "../tools.h"

FILE *__logFile = NULL;

int WINAPI WinMain( HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow ) {
  fopen_s( &__logFile, "log.txt", "wb+" );
  Engine::EntryPointWin32 *entryPoint = new Engine::EntryPointWin32();
  Engine::Core *core = new Engine::CoreWin32( hInstance, hPrevInstance, lpCmdLine, nCmdShow );
  entryPoint->Run( core );
  SAFE_DELETE( core );
  SAFE_DELETE( entryPoint );
  fclose( __logFile );
  __logFile = NULL;
}

using namespace Engine;

EntryPointWin32::EntryPointWin32()
:EntryPoint() {
}

EntryPointWin32::~EntryPointWin32() {
}
