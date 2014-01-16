#pragma once

#include <android/log.h>
//#include <memory>

#define SAFE_DELETE( q ) { if( q ) { delete q; q = 0; } }
#define SAFE_DELETE_ARRAY( q ) { if( q ) { delete[] q; q = 0; }  }

#define LOGI(...) ((void)__android_log_print(ANDROID_LOG_INFO, "native-activity", __VA_ARGS__))
#define LOGW(...) ((void)__android_log_print(ANDROID_LOG_WARN, "native-activity", __VA_ARGS__))
