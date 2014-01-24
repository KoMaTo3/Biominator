#include "core.h"
#include "renderer.h"
#include "tools.h"
#include "android/sensor.h"

using namespace Engine;

CoreAndroid::CoreAndroid( android_app *setApp )
:Core(), app( setApp ), sensorManager( NULL ), sensorAccelometer( NULL ), sensorEventQueue( NULL ) {
  app_dummy();
  this->app->onAppCmd = CoreAndroid::HandleCmd;
  this->app->onInputEvent = CoreAndroid::HandleInput;
  this->app->userData = this;
  this->app->inputPollSource.process = CoreAndroid::ProcessInput;

  //this->sensorManager = ASensorManager_getInstance();
  //this->sensorAccelometer = ASensorManager_getDefaultSensor( this->sensorManager, ASENSOR_TYPE_ACCELEROMETER );
  //this->sensorEventQueue = ASensorManager_createEventQueue( this->sensorManager, this->app->looper, LOOPER_ID_USER, NULL, NULL );
  //this->savedState.angle = 0.0f;
  //this->savedState.x = 0;
  //this->savedState.y = 0;

  if( this->app->savedState ) {
    //this->savedState = *( ( AndroidSavedState* ) this->app->savedState );
  }
}


CoreAndroid::~CoreAndroid() {
  SAFE_DELETE( this->renderer );
}

void CoreAndroid::Update() {
  int ident;
  int events;
  struct android_poll_source* source;

  while( ( ident = ALooper_pollAll( this->isFocused && this->isVisible && this->renderer ? 0 : -1, NULL, &events, ( void** ) &source ) ) >= 0 ) {
    if( source != NULL ) {
      source->process( this->app, source );
    }

    /*
    if( ident == LOOPER_ID_USER ) {
        if( this->sensorAccelometer ) {
            ASensorEvent event;
            while( ASensorEventQueue_getEvents( this->sensorEventQueue, &event, 1 ) > 0 ) {
              //LOGI("accelerometer: x=%f y=%f z=%f", event.acceleration.x, event.acceleration.y, event.acceleration.z);
            }
        }
    }
    */

    if( this->app->destroyRequested != 0 ) {
      //SAFE_DELETE( this->renderer );
      this->isValid = false;
      //this->isVisible = false;
      //this->isFocused = false;
      LOGE( "destroyRequested" );
      break;
      //this->Destroy();
      //break;
    }
  }//poll
}//Update

ANativeWindow* CoreAndroid::GetWindow() {
  return ( this->isValid && this->app ? this->app->window : NULL );
}//GetWindow

void CoreAndroid::Destroy() {
  //SAFE_DELETE( this->renderer );
  this->isValid = false;
}//Destroy

void CoreAndroid::HandleCmd( struct android_app* app, int cmd ) {
  CoreAndroid *core = ( CoreAndroid* ) app->userData;
  if( !core ) {
    return;
  }
  LOGE( "HandleCmd" );
  switch( cmd ) {
    case APP_CMD_SAVE_STATE: {
      LOGE( "HandleCmd => APP_CMD_SAVE_STATE" );
      core->app->savedState = 0;
      core->app->savedStateSize = 0;
      /*
      core->app->savedState = new AndroidSavedState();
      *( ( AndroidSavedState* ) core->app->savedState ) = core->savedState;
      core->app->savedStateSize = sizeof( core->savedState );
      */
    }
    break;
    case APP_CMD_INIT_WINDOW: {
      LOGE( "HandleCmd => APP_CMD_INIT_WINDOW" );
      if( core ) {
        core->renderer = new Engine::RendererAndroid( core->app->window, core->screenWidth, core->screenHeight );
      } else {
        LOGE( "APP_CMD_INIT_WINDOW => core is NULL" );
      }
      /*
      if( core && core->app && core->app->window ) {
        if( core->renderer ) {
          LOGE( "ERROR: APP_CMD_INIT_WINDOW => renderer already exists" );
        } else {
          core->renderer = new Engine::RendererAndroid( core->app->window, core->screenWidth, core->screenHeight );
        }
      }
      */
    }
    break;
    case APP_CMD_TERM_WINDOW: {
      LOGE( "HandleCmd => APP_CMD_TERM_WINDOW" );
      SAFE_DELETE( core->renderer );
      //core->isVisible = false;
      //core->isFocused = false;
      //core->Destroy();
    }
    break;
    case APP_CMD_GAINED_FOCUS: {
      LOGE( "HandleCmd => APP_CMD_GAINED_FOCUS" );
      core->isFocused = true;
      /*
      if( core->sensorAccelometer ) {
        ASensorEventQueue_enableSensor( core->sensorEventQueue, core->sensorAccelometer );
        ASensorEventQueue_setEventRate( core->sensorEventQueue, core->sensorAccelometer, ( 1000L / 60 )*1000 );
      }
      */
    }
    break;
    case APP_CMD_LOST_FOCUS: {
      core->isFocused = false;
      //pause game there
      //core->Destroy();
      LOGE( "HandleCmd => APP_CMD_LOST_FOCUS" );
      /*
      if( core->sensorAccelometer ) {
        ASensorEventQueue_disableSensor( core->sensorEventQueue, core->sensorAccelometer );
      }
      */
    }
    break;
    case APP_CMD_START: {
      LOGE( "HandleCmd => APP_CMD_START" );
      //core->isVisible = true;
      //load resources there
    }
    break;
    case APP_CMD_STOP: {
      LOGE( "HandleCmd => APP_CMD_STOP" );
      //SAFE_DELETE( core->renderer );
      //save game there
    }
    break;
    case APP_CMD_RESUME: {
      LOGE( "HandleCmd => APP_CMD_RESUME" );
      core->isVisible = true;
    }
    break;
    case APP_CMD_PAUSE: {
      LOGE( "HandleCmd => APP_CMD_PAUSE" );
      core->isVisible = false;
    }
    break;
    /*
    case APP_CMD_DESTROY: {
      LOGE( "HandleCmd => APP_CMD_DESTROY" );
      //core->Destroy();
    }
    break;
    case APP_CMD_INPUT_CHANGED: {
      LOGE( "HandleCmd => APP_CMD_INPUT_CHANGED" );
    }
    break;
    */
    default: {
      LOGE( "HandleCmd => x%X", cmd );
    }
    break;
  }
}//HandleCmd

void CoreAndroid::ProcessInput( struct android_app* app, struct android_poll_source *source ) {
  AInputEvent* event = NULL;
  int processed = 0;
  while( AInputQueue_hasEvents( app->inputQueue ) && AInputQueue_getEvent( app->inputQueue, &event ) >= 0 ) {
    LOGE( "New input event: type=%d\n", AInputEvent_getType( event ) );
    bool skipPredispatch =
      AInputEvent_getType( event ) == AINPUT_EVENT_TYPE_KEY
      /* && AKeyEvent_getKeyCode( event ) == AKEYCODE_BACK */ ;
    if( !skipPredispatch && AInputQueue_preDispatchEvent( app->inputQueue, event ) ) {
      continue;
    }
    int32_t handled = 0;
    if( app->onInputEvent != NULL ) {
      LOGE( "app->onInputEvent..." );
      handled = app->onInputEvent(app, event);
    }
    AInputQueue_finishEvent( app->inputQueue, event, handled );
    processed = 1;
  }
  if (processed == 0) {
    LOGE("Failure reading next input event: %s\n", strerror(errno));
  }
}//ProcessInput

int32_t CoreAndroid::HandleInput( struct android_app* app, AInputEvent *event ) {
  Engine::CoreAndroid *core = ( Engine::CoreAndroid* ) app->userData;

  int32_t eventType = AInputEvent_getType( event );
  if( eventType == AINPUT_EVENT_TYPE_MOTION ) {
      //engine->animating = 1; engine->state.x = AMotionEvent_getX(event, 0); engine->state.y = AMotionEvent_getY(event, 0);

      return 1;
      //return handle_touch_input(event);
  } else if( eventType == AINPUT_EVENT_TYPE_KEY ) {
    if( AKeyEvent_getKeyCode( event ) == AKEYCODE_BACK ) {
      LOGE( "Do exit by key" );
      core->isValid = false;
      return 1;
    }
    //return handle_key_input(event);
  }

  return 0;
}//HandleInput
