#include "entrypoint.h"
#include "core.h"
#include "../tools.h"

void android_main( struct android_app* state ) {
  Engine::EntryPointAndroid *entryPoint = new Engine::EntryPointAndroid();
  entryPoint->Run( new Engine::CoreAndroid( state ) );
  delete entryPoint;
  LOGE( "ANativeActivity_finish" );
  ANativeActivity_finish( state->activity );
}

using namespace Engine;

EntryPointAndroid::EntryPointAndroid()
:EntryPoint() {
}

EntryPointAndroid::~EntryPointAndroid() {
}
