#include "entrypoint.h"
#include "core.h"

FILE *__logFile = NULL;

int WINAPI WinMain( HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow ) {
  fopen_s( &__logFile, "log.txt", "wb+" );
  Engine::EntryPointWin32 *entryPoint = new Engine::EntryPointWin32();
  entryPoint->Run( new Engine::CoreWin32( hInstance, hPrevInstance, lpCmdLine, nCmdShow ) );
  delete entryPoint;
  fclose( __logFile );
  __logFile = NULL;
}

using namespace Engine;

EntryPointWin32::EntryPointWin32()
:EntryPoint() {
}

EntryPointWin32::~EntryPointWin32() {
}
