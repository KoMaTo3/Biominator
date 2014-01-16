#pragma once

#include "../core.h"

#include <jni.h>
#include <errno.h>

#include "stdint.h"
#include <android_native_app_glue.h>

namespace Engine {

class RendererAndroid;

class CoreAndroid: public Core {
public:
  CoreAndroid( android_app *setState );
  virtual ~CoreAndroid();
  virtual void Update();
  virtual void Destroy();
  static void HandleCmd( struct android_app* app, int cmd );
  static int32_t HandleInput( struct android_app* app, AInputEvent *event );
  ANativeWindow* GetWindow();

private:
  CoreAndroid();
  CoreAndroid( const CoreAndroid& );
  CoreAndroid& operator=( const CoreAndroid& );

  android_app *state;
};

};
