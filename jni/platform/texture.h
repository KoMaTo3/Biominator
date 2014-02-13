#pragma once

#include "../predefines.h"

#if IS_WIN32

#include "../win32/texture.h"

#elif IS_ANDROID

#include "../android/texture.h"

#elif IS_LINUX

#include "../linux/texture.h"

#endif
