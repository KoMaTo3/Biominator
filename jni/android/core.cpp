#include "core.h"
#include "renderer.h"
#include "../tools.h"
#include "../types.h"
#include "../eventtypes.h"

using namespace Engine;

CoreAndroid::CoreAndroid( android_app *setApp )
:Core(), app( setApp ), doFinish( false ) {
  app_dummy();
  this->app->onAppCmd = CoreAndroid::HandleCmd;
  this->app->onInputEvent = CoreAndroid::HandleInput;
  this->app->userData = this;
  this->app->inputPollSource.process = CoreAndroid::ProcessInput;

  //this->savedState.angle = 0.0f;
  //this->savedState.x = 0;
  //this->savedState.y = 0;

  if( this->app->savedState ) {
    this->savedState = *( ( AndroidSavedState* ) this->app->savedState );
  }
}

CoreAndroid::~CoreAndroid() {
  //SAFE_DELETE( this->renderer );
}

void CoreAndroid::Update() {
  int ident;
  int events;
  struct android_poll_source* source;

  while( ( ident = ALooper_pollAll( this->animating ? 0 : -1, NULL, &events, ( void** ) &source ) ) >= 0 ) {
    if( source != NULL ) {
      source->process( this->app, source );
    }

    if( this->app->destroyRequested != 0 ) {
      this->Destroy();
      break;
    }
  }//poll
  if( this->doFinish ) {
    this->doFinish = false;
    ANativeActivity_finish( this->app->activity );
  }
}//Update

void CoreAndroid::Destroy() {
  this->isValid = false;
  this->isVisible = false;
  this->isFocused = false;
  this->animating = false;
}//Destroy

void CoreAndroid::HandleCmd( struct android_app* app, int cmd ) {
  CoreAndroid *core = ( CoreAndroid* ) app->userData;
  switch( cmd ) {
    case APP_CMD_SAVE_STATE: {
      core->app->savedState = new AndroidSavedState();
      *( ( AndroidSavedState* ) core->app->savedState ) = core->savedState;
      core->app->savedStateSize = sizeof( core->savedState );
    }
    break;
    case APP_CMD_INIT_WINDOW: {
      SAFE_DELETE( core->renderer );
      core->renderer = new Engine::RendererAndroid( core->app->window, core->screenWidth, core->screenHeight );
      core->animating = core->isVisible && core->isFocused;
    }
    break;
    case APP_CMD_TERM_WINDOW: {
      //SAFE_DELETE( core->renderer );  //!!!
      core->animating = false;
      core->isVisible = false;
      core->isFocused = false;
    }
    break;
    case APP_CMD_GAINED_FOCUS: {
      core->isFocused = true;
      core->animating = core->isFocused && core->isVisible;
    }
    break;
    case APP_CMD_LOST_FOCUS: {
      core->isFocused = false;
      core->animating = false;
      //pause game there
    }
    break;
    case APP_CMD_START: {
      core->isVisible = true;
      core->animating = core->isFocused && core->isVisible;
      //load resources there
    }
    break;
    case APP_CMD_STOP: {
      core->isVisible = false;
      core->animating = false;
      //save game there
    }
    break;
    case APP_CMD_RESUME: {
      core->isVisible = true;
      core->animating = core->isFocused && core->isVisible;
    }
    break;
    case APP_CMD_PAUSE: {
      core->isVisible = false;
      core->animating = false;
    }
    break;
    case APP_CMD_DESTROY: {
    }
    break;
    case APP_CMD_INPUT_CHANGED: {
    }
    break;
    case APP_CMD_CONFIG_CHANGED: {
    }
    break;
  }
}//HandleCmd

void CoreAndroid::ProcessInput( struct android_app* app, struct android_poll_source *source ) {
  AInputEvent* event = NULL;
  int processed = 0;
  while( AInputQueue_hasEvents( app->inputQueue ) && AInputQueue_getEvent( app->inputQueue, &event ) >= 0 ) {
    bool skipPredispatch = AInputEvent_getType( event ) == AINPUT_EVENT_TYPE_KEY;
    if( !skipPredispatch && AInputQueue_preDispatchEvent( app->inputQueue, event ) ) {
      continue;
    }
    int32_t handled = 0;
    if( app->onInputEvent != NULL ) {
      handled = app->onInputEvent(app, event);
    }
    AInputQueue_finishEvent( app->inputQueue, event, handled );
    processed = 1;
  }
  if (processed == 0) {
    LOGE( "Failure reading next input event: %s\n", strerror( errno ) );
  }
}//ProcessInput

int32_t CoreAndroid::HandleInput( struct android_app* app, AInputEvent *event ) {
  Engine::CoreAndroid *core = ( Engine::CoreAndroid* ) app->userData;

  int32_t eventType = AInputEvent_getType( event );
  if( eventType == AINPUT_EVENT_TYPE_MOTION ) {
    core->animating = true;
    size_t pointerCount = AMotionEvent_getPointerCount( event );
    int32_t action = AMotionEvent_getAction( event );
    for( int index = 0; index < pointerCount; ++index ) {
      //int32_t index = ( action & AMOTION_EVENT_ACTION_POINTER_INDEX_MASK ) >> AMOTION_EVENT_ACTION_POINTER_INDEX_SHIFT;
      Position pos( AMotionEvent_getX( event, index ), AMotionEvent_getY( event, index ) );
      core->TouchEvent( EventType::EVENT_TYPE_MOTION, &pos );
      int32_t id = AMotionEvent_getPointerId( event, index );
      LOGI( "Motion: %d, %d [%d]", pos.x, pos.y, id );
    }
    return 1;
  } else if( eventType == AINPUT_EVENT_TYPE_KEY ) {
    if( AKeyEvent_getKeyCode( event ) == AKEYCODE_BACK ) {
      //ANativeActivity_finish( app->activity );
      core->doFinish = true;
    }
    return 1;
  }

  return 0;
}//HandleInput
