LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

#LOCAL_CFLAGS 		:= -D__GXX_EXPERIMENTAL_CXX0X__
LOCAL_MODULE    := Biominator
LOCAL_SRC_FILES := Biominator.cpp core.cpp android/core.cpp renderer.cpp android/renderer.cpp
#LOCAL_CPPFLAGS  := -std=c++11
#APP_CFLAGS += -std=c++11
#APP_STL := stlport_static

LOCAL_LDLIBS:= -landroid -llog -lEGL -lGLESv2
LOCAL_STATIC_LIBRARIES:= android_native_app_glue
APP_PLATFORM := android-9

include $(BUILD_SHARED_LIBRARY)
$(call import-module,android/native_app_glue)
