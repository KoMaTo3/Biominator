#include "android/core.h"
#include "android/renderer.h"
#include "tools.h"

void android_main(struct android_app* state)
{
  app_dummy();
  Engine::CoreAndroid *core = new Engine::CoreAndroid( state );
  core->Run();
  SAFE_DELETE( core );
}
