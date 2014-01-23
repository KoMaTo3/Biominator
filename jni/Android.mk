LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

#LOCAL_CFLAGS 		:= -D__GXX_EXPERIMENTAL_CXX0X__
LOCAL_MODULE    := Biominator
LOCAL_SRC_FILES :=  Biominator.cpp \
                    core.cpp \
                    android/core.cpp \
                    renderer.cpp \
                    android/renderer.cpp \
                    entrypoint.cpp \
                    android/entrypoint.cpp
#LOCAL_CPPFLAGS  := -std=c++11
#APP_CFLAGS += -std=c++11

LOCAL_LDLIBS:= -landroid -llog -lEGL -lGLESv2
LOCAL_STATIC_LIBRARIES:= android_native_app_glue
#LOCAL_SHARED_LIBRARIES += libstlport
#LOCAL_C_INCLUDES += ${NDKROOT}/sources/cxx-stl/stlport/stlport
APP_STL := stlport_static

include $(BUILD_SHARED_LIBRARY)
$(call import-module,android/native_app_glue)
