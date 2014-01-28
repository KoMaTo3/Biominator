#include "entrypoint.h"
#include "core.h"
#include "../tools.h"

void android_main( struct android_app* state ) {
  Engine::EntryPointAndroid *entryPoint = new Engine::EntryPointAndroid();
  Engine::Core *core = new Engine::CoreAndroid( state );
  entryPoint->Run( core );
  SAFE_DELETE( core );
  SAFE_DELETE( entryPoint );
  ANativeActivity_finish( state->activity );
}

using namespace Engine;

EntryPointAndroid::EntryPointAndroid()
:EntryPoint() {
}

EntryPointAndroid::~EntryPointAndroid() {
}
