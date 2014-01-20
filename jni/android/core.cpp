#include "core.h"
#include "renderer.h"
#include "tools.h"

using namespace Engine;

CoreAndroid::CoreAndroid( android_app *setState )
:Core(), state( setState ) {
  app_dummy();
  this->state->onAppCmd = CoreAndroid::HandleCmd;
  this->state->onInputEvent = CoreAndroid::HandleInput;
  this->state->userData = this;
}


CoreAndroid::~CoreAndroid() {
}

void CoreAndroid::Update() {
  int ident;
  int events;
  struct android_poll_source* source;

  while( this->isValid && ( ident = ALooper_pollAll( 0, NULL, &events, ( void** ) &source ) ) >= 0 ) {
    if( source != NULL ) {
      source->process( this->state, source );
    }

    // If a sensor has data, process it now.
    /*
    if (ident == LOOPER_ID_USER) {
        if (engine.accelerometerSensor != NULL) {
            ASensorEvent event;
            while (ASensorEventQueue_getEvents(engine.sensorEventQueue,
                    &event, 1) > 0) {
                LOGI("accelerometer: x=%f y=%f z=%f",
                        event.acceleration.x, event.acceleration.y,
                        event.acceleration.z);
            }
        }
    }
    */

    if( this->state->destroyRequested != 0 ) {
      this->Destroy();
      break;
    }
  }
}//Update

ANativeWindow* CoreAndroid::GetWindow() {
  return ( this->isValid && this->state ? this->state->window : NULL );
}//GetWindow

void CoreAndroid::Destroy() {
  this->isValid = false;
}//Destroy

void CoreAndroid::HandleCmd( struct android_app* app, int cmd ) {
  CoreAndroid *core = ( CoreAndroid* ) app->userData;
  LOGI( "HandleCmd" );
  switch( cmd ) {
    case APP_CMD_INIT_WINDOW: {
      LOGI( "HandleCmd => APP_CMD_INIT_WINDOW, window[%p]", core->GetWindow() );
      core->renderer = new Engine::RendererAndroid( core->GetWindow() );
    }
    break;
    case APP_CMD_DESTROY: {
      LOGI( "HandleCmd => APP_CMD_DESTROY" );
      core->Destroy();
    }
    break;
    case APP_CMD_TERM_WINDOW: {
      LOGI( "HandleCmd => APP_CMD_TERM_WINDOW" );
      core->Destroy();
    }
    break;
    case APP_CMD_RESUME: {
      LOGI( "HandleCmd => APP_CMD_RESUME" );
    }
    break;
    case APP_CMD_PAUSE: {
      LOGI( "HandleCmd => APP_CMD_PAUSE" );
    }
    break;
    case APP_CMD_STOP: {
      LOGI( "HandleCmd => APP_CMD_STOP" );
    }
    break;
    case APP_CMD_LOST_FOCUS: {
      LOGI( "HandleCmd => APP_CMD_LOST_FOCUS" );
    }
    break;
    case APP_CMD_INPUT_CHANGED: {
      LOGI( "HandleCmd => APP_CMD_INPUT_CHANGED" );
    }
    break;
    case APP_CMD_GAINED_FOCUS: {
      LOGI( "HandleCmd => APP_CMD_GAINED_FOCUS" );
    }
    break;
    case APP_CMD_START: {
      LOGI( "HandleCmd => APP_CMD_START" );
    }
    break;
    default: {
      LOGI( "HandleCmd => x%X", cmd );
    }
    break;
  }
}//HandleCmd

int32_t CoreAndroid::HandleInput( struct android_app* app, AInputEvent *event ) {
  while( AInputQueue_hasEvents( app->inputQueue ) ) {
    if( AInputQueue_getEvent( app->inputQueue, &event ) >= 0 ) {
      int32_t result = 0;
      //uint32_t devId = AInputEvent_getDeviceId( event );
      //uint32_t src = AInputEvent_getSource( event );

      int32_t predispatched = ( AInputQueue_preDispatchEvent( app->inputQueue, event ) );
      if( !predispatched ) {
        int32_t type = AInputEvent_getType( event );
        CoreAndroid *core = ( CoreAndroid* ) app->userData;
        switch( type ) {
          case AINPUT_EVENT_TYPE_KEY: {
            result = true;
            break;
          }
        }//switch type
      }
      if( !predispatched ) {
        AInputQueue_finishEvent( app->inputQueue, event, result );
      }
      //AInputQueue_preDispatchEvent( app->inputQueue, event );
      //AInputQueue_finishEvent( core->state->inputQueue, event, result );
    }//if
  }//while
  return 0;
}//HandleInput
