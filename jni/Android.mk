LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

LOCAL_MODULE    := Biominator
LOCAL_SRC_FILES :=  Biominator.cpp \
										camera.cpp \
										camera2d.cpp \
										camera3d.cpp \
                    core.cpp \
                    entrypoint.cpp \
                    event.cpp \
                    eventmanager.cpp \
                    eventmanagercontainer.cpp \
                    eventtypes.cpp \
                    filemanager.cpp \
                    imageloader.cpp \
                    listener.cpp \
                    material.cpp \
                    math/kangles.cpp \
                    math/klib.cpp \
                    math/kmath.cpp \
                    math/kmatrix.cpp \
                    math/kquat.cpp \
                    math/kvector.cpp \
                    memory.cpp \
                    mesh.cpp \
                    producer.cpp \
                    renderer.cpp \
                    shadermanager.cpp \
                    shaderprogram.cpp \
                    texture.cpp \
                    tools.cpp \
                    types.cpp \
                    vertexbuffer.cpp \
                    vertexbuffergl.cpp \
                    android/core.cpp \
                    android/entrypoint.cpp \
                    android/filemanager.cpp \
                    android/renderer.cpp \
                    android/texture.cpp
#LOCAL_CFLAGS += -std=c++11
LOCAL_CFLAGS += -std=gnu++11

LOCAL_LDLIBS:= -landroid -llog -lEGL -lGLESv2
LOCAL_STATIC_LIBRARIES:= android_native_app_glue
APP_STL := stlport_static

include $(BUILD_SHARED_LIBRARY)
$(call import-module,android/native_app_glue)
