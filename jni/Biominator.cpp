#include "predefines.h"

#if IS_WIN32
#include "win32/core.h"
#include "win32/renderer.h"
#elif IS_ANDROID
#include "android/core.h"
#include "android/renderer.h"
#endif

#include "tools.h"

ENGINE_MAIN {
  Engine::ENGINE_CORE *core = new Engine::ENGINE_CORE( ENGINE_CORE_ARGUMENTS );
  core->Run();
  SAFE_DELETE( core );
}
