#pragma once

#include "../core.h"

#include <jni.h>
#include <errno.h>

#include "stdint.h"
#include <android_native_app_glue.h>

namespace Engine {

class RendererAndroid;
struct AndroidSavedState {
  float angle;
  int32_t x;
  int32_t y;
};

class CoreAndroid: public Core {
public:
  CoreAndroid( android_app *setApp );
  virtual ~CoreAndroid();
  virtual void Update();
  virtual void Destroy();
  virtual void Stop();
  static void HandleCmd( struct android_app* app, int cmd );
  static int32_t HandleInput( struct android_app* app, AInputEvent *event );
  static void ProcessInput( struct android_app* app, struct android_poll_source *source );

private:
  CoreAndroid();
  CoreAndroid( const CoreAndroid& );
  CoreAndroid& operator=( const CoreAndroid& );

  android_app *app;
  AndroidSavedState savedState;
  bool doFinish;
};

};
