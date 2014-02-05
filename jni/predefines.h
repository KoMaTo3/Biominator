#pragma once

#define IS_WIN32    ( 0 )
#define IS_ANDROID  ( 0 )
#define IS_LINUX    ( 0 )

#ifdef _WIN32

#undef IS_WIN32
#define IS_WIN32 (1)

#elif defined __ANDROID__

#undef IS_ANDROID
#define IS_ANDROID (1)

#elif defined __linux

#undef IS_LINUX
#define IS_LINUX (1)

#endif
