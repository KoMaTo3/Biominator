#include "predefines.h"

#if IS_WIN32
#include "win32/entrypoint.h"
#elif IS_ANDROID
#include "android/entrypoint.h"
#elif IS_LINUX
#include "linux/entrypoint.h"
#endif

#include "tools.h"
#include "listener.h"
#include "eventtypes.h"
#include "producer.h"

//test
#include "vertexbuffergl.h"
#include "memory.h"
#include "filemanager.h"

namespace Game {

class KeyListener;

class GameContainer: public Engine::Listener {
public:
  GameContainer( Engine::Core *setCore );
  virtual ~GameContainer();
  static void OnKeyEvent( Engine::Listener* listener, Engine::Producer *producer, int eventId, void *data );
  static void OnMobileKeyPressed( Engine::Listener* listener, Engine::Producer *producer, int eventId, void *data );
  static void OnAppClose( Engine::Listener* listener, Engine::Producer *producer, int eventId, void *data );
  static void OnGraphicsInit( Engine::Listener* listener, Engine::Producer *producer, int eventId, void *data );
  void AppExit();
  void AppSuspend();
  void _TestFile();

  Engine::Core *core;

private:
  GameContainer();
  GameContainer( GameContainer& );
  GameContainer& operator=( GameContainer& );

  //test
  Engine::VertexBufferGL *buffer;
};

GameContainer *game = NULL;

};  //namespace Game

using namespace Game;


GameContainer::GameContainer( Engine::Core *setCore )
:core( setCore ), buffer( NULL ) {
  switch( this->core->GetPlatformType() ) {
    case Engine::PLATFORM_TYPE_WIN32: {
      this->core->Listen( this, Engine::EVENT_TYPE_KEY_PRESSED, GameContainer::OnKeyEvent );
      this->core->Listen( this, Engine::EVENT_TYPE_KEY_RELEASED, GameContainer::OnKeyEvent );
      this->core->Listen( this, Engine::EVENT_TYPE_KEY_CLICKED, GameContainer::OnKeyEvent );
      this->core->Listen( this, Engine::EVENT_TYPE_KEY_DBLCLICKED, GameContainer::OnKeyEvent );
    }
    break;
    case Engine::PLATFORM_TYPE_ANDROID: {
      this->core->Listen( this, Engine::EVENT_TYPE_SYSTEM_KEY, GameContainer::OnKeyEvent );
    }
    break;
    case Engine::PLATFORM_TYPE_LINUX: {
    }
    break;
    default: {
      LOGE( "GameContainer: unknown platform" );
    }
    break;
  }
  this->core->Listen( this, Engine::EVENT_TYPE_CORE_CLOSE, GameContainer::OnAppClose );
  this->core->Listen( this, Engine::EVENT_TYPE_RENDERER_INIT, GameContainer::OnGraphicsInit );

  this->_TestFile();
}

GameContainer::~GameContainer() {
  SAFE_DELETE( this->buffer );
}

void GameContainer::OnKeyEvent( Engine::Listener* listener, Engine::Producer *producer, int eventId, void *data ) {
  Engine::EventKey *event = ( Engine::EventKey* ) data;
  GameContainer *game = ( GameContainer* ) listener;
  if( event->keyCode == Engine::KEY_CODE_ESCAPE ) {
    game->AppExit();
  }
  if( event->keyCode == Engine::KEY_CODE_BACKSPACE ) {
    game->AppSuspend();
  }
}//OnKeyEvent

void GameContainer::AppExit() {
  this->core->Destroy();
}//AppExit

void GameContainer::AppSuspend() {
  this->core->Stop();
}//AppSuspend

void GameContainer::OnAppClose( Engine::Listener* listener, Engine::Producer *producer, int eventId, void *data ) {
  GameContainer *game = ( GameContainer* ) listener;
  game->AppExit();
}//OnAppClose


//entry point
void Engine::EntryPoint::Init( Core* core ) {
  game = new GameContainer( core );
}//Init

void Engine::EntryPoint::Destroy() {
  SAFE_DELETE( game );
}//Destroy

void GameContainer::OnGraphicsInit( Engine::Listener* listener, Engine::Producer *producer, int eventId, void *data ) {
  GameContainer *game = ( GameContainer* ) listener;
  game->buffer = new Engine::VertexBufferGL( game->core->renderer );
  game->buffer->New( 1204 );
  game->buffer->New( 3769 );
}//OnGraphicsInit

void GameContainer::_TestFile() {
  Engine::Memory testmem;
  if( this->core->GetFileManager()->GetFile( "qwe.tx", testmem, true ) ) {
    LOGI( "File finded: size[%d] content[%s]", testmem.GetLength(), testmem.GetData() );
  } else {
    LOGE( "File not found" );
  }
}//_TestFile
