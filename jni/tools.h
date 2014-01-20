#pragma once

#include "predefines.h"

#define SAFE_DELETE( q ) { if( q ) { delete q; q = 0; } }
#define SAFE_DELETE_ARRAY( q ) { if( q ) { delete[] q; q = 0; }  }

#if IS_WIN32
#define ENGINE_MAIN int WINAPI WinMain( HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow )
#define ENGINE_CORE CoreWin32
#define ENGINE_CORE_ARGUMENTS hInstance, hPrevInstance, lpCmdLine, nCmdShow
#define LOGI(format, ...) (printf(format, __VA_ARGS__))
#define LOGW(format, ...) (printf("Warning: " format, __VA_ARGS__))
#define LOGE(format, ...) (printf("Error: " format, __VA_ARGS__))
//_WIN32
#elif IS_ANDROID
#define ENGINE_MAIN void android_main( struct android_app* state )
#define ENGINE_CORE CoreAndroid
#define ENGINE_CORE_ARGUMENTS state
#include <android/log.h>
#define LOGI(...) ((void)__android_log_print(ANDROID_LOG_INFO, "native-activity", __VA_ARGS__))
#define LOGW(...) ((void)__android_log_print(ANDROID_LOG_WARN, "native-activity", __VA_ARGS__))
#define LOGE(...) ((void)__android_log_print(ANDROID_LOG_ERROR, "native-activity", __VA_ARGS__))
//__ANDROID__
#endif
//platform case
