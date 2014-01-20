#include "predefines.h"

#if IS_WIN32
#include "win32/entrypoint.h"
#elif IS_ANDROID
#include "android/entrypoint.h"
#endif

#include "tools.h"

void Engine::EntryPoint::Init( Core* core ) {
}
