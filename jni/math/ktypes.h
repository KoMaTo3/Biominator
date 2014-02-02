#pragma once

#if IS_WIN32
#define KM_INLINE __forceinline
#else
#define KM_INLINE inline
#endif
