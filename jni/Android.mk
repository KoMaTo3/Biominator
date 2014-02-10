LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

LOCAL_MODULE    := Biominator
LOCAL_SRC_FILES :=  Biominator.cpp \
                    core.cpp \
                    entrypoint.cpp \
                    event.cpp \
                    eventmanager.cpp \
                    eventmanagercontainer.cpp \
                    filemanager.cpp \
                    imageloader.cpp \
                    listener.cpp \
                    memory.cpp \
                    producer.cpp \
                    renderer.cpp \
                    types.cpp \
                    vertexbuffer.cpp \
                    vertexbuffergl.cpp \
                    android/core.cpp \
                    android/entrypoint.cpp \
                    android/filemanager.cpp \
                    android/renderer.cpp
LOCAL_CFLAGS += -std=c++11

LOCAL_LDLIBS:= -landroid -llog -lEGL -lGLESv2
LOCAL_STATIC_LIBRARIES:= android_native_app_glue
APP_STL := stlport_static

include $(BUILD_SHARED_LIBRARY)
$(call import-module,android/native_app_glue)
