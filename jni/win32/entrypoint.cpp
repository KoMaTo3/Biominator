#include "entrypoint.h"
#include "core.h"

int WINAPI WinMain( HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow ) {
  Engine::EntryPointWin32 *entryPoint = new Engine::EntryPointWin32();
  entryPoint->Run( new Engine::CoreWin32( hInstance, hPrevInstance, lpCmdLine, nCmdShow ) );
  delete entryPoint;
}

using namespace Engine;

EntryPointWin32::EntryPointWin32()
:EntryPoint() {
}

EntryPointWin32::~EntryPointWin32() {
}
