#pragma once

#include "stdio.h"
#include "predefines.h"
#include <deque>
#include <string>

#define SAFE_DELETE( q ) { if( q ) { delete q; q = 0; } }
#define SAFE_DELETE_ARRAY( q ) { if( q ) { delete[] q; q = 0; }  }
#define COLOR_RGBA( r, g, b, a ) ( (r) | (g<<8) | (b<<16) | a<<24 )
#define COLOR_ARGB( a, r, g, b ) ( (a<<24) | (r<<16) | (g<<8) | b )


#if IS_WIN32
#define ENGINE_MAIN int WINAPI WinMain( HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow )
#define ENGINE_CORE CoreWin32
#define ENGINE_CORE_ARGUMENTS hInstance, hPrevInstance, lpCmdLine, nCmdShow
#define ENGINE_PLATFORM_NAME "win32"
#define ENGINE_PLATFORM_TYPE PLATFORM_TYPE_WIN32
#define ENGINE_PLATFORM_INCLUDE_OPENGL "win32/opengl.h"
#define LOGI(format, ...) ( fprintf( __logFile, format "\n", __VA_ARGS__ ) ); fflush( __logFile )
#define LOGW(format, ...) ( fprintf( __logFile, "[W] " format "\n", __VA_ARGS__ ) ); fflush( __logFile )
#define LOGE(format, ...) ( fprintf( __logFile, "[E] " format "\n", __VA_ARGS__ ) ); fflush( __logFile )
extern FILE *__logFile;
//_WIN32
#elif IS_ANDROID
#define ENGINE_MAIN void android_main( struct android_app* state )
#define ENGINE_CORE CoreAndroid
#define ENGINE_CORE_ARGUMENTS state
#define ENGINE_PLATFORM_NAME "android"
#define ENGINE_PLATFORM_TYPE PLATFORM_TYPE_ANDROID
#define ENGINE_PLATFORM_INCLUDE_OPENGL "android/opengl.h"
#include <android/log.h>
#define LOGI(...) ((void)__android_log_print(ANDROID_LOG_INFO, "native-activity", __VA_ARGS__))
#define LOGW(...) ((void)__android_log_print(ANDROID_LOG_WARN, "native-activity", __VA_ARGS__))
#define LOGE(...) ((void)__android_log_print(ANDROID_LOG_ERROR, "native-activity", __VA_ARGS__))
#define __need_size_t
#include <stddef.h>
//__ANDROID__
#elif IS_LINUX
#define ENGINE_MAIN void main()
#define ENGINE_CORE CoreLinux
#define ENGINE_CORE_ARGUMENTS void
#define ENGINE_PLATFORM_NAME "linux"
#define ENGINE_PLATFORM_TYPE PLATFORM_TYPE_LINUX
#define ENGINE_PLATFORM_INCLUDE_OPENGL "linux/opengl.h"
#define LOGI(format, ...) ( printf( format"\n", ##__VA_ARGS__ ) )
#define LOGW(format, ...) ( printf( "[Warn] " format"\n", ##__VA_ARGS__ ) )
#define LOGE(format, ...) ( printf( "[Error] " format"\n", ##__VA_ARGS__ ) )
extern FILE *__logFile;
//__ANDROID__
#endif
//platform case

std::deque< std::string > Explode( const std::string& source, const std::string& separator );
