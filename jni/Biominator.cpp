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
#include "imageloader.h"
#include "types.h"
#include "mesh.h"
#include "renderer.h"
#include "material.h"
#include "shaderprogram.h"

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
  static void OnAfterRender( Engine::Listener* listener, Engine::Producer *producer, int eventId, void *data );
  static void OnInitRender( Engine::Listener* listener, Engine::Producer *producer, int eventId, void *data );
  void AppExit();
  void AppSuspend();
  void _TestFile();
  void _TestImageLoader();
  void _TestTexture();
  void _TestMesh();

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
      this->core->renderer->Listen( this, Engine::EVENT_TYPE_RENDERER_AFTER_RENDER, GameContainer::OnAfterRender );
      this->core->renderer->Listen( this, Engine::EVENT_TYPE_RENDERER_INIT, GameContainer::OnInitRender );
    }
    break;
    case Engine::PLATFORM_TYPE_ANDROID: {
      this->core->Listen( this, Engine::EVENT_TYPE_SYSTEM_KEY, GameContainer::OnKeyEvent );
    }
    break;
    case Engine::PLATFORM_TYPE_LINUX: {
      this->core->Listen( this, Engine::EVENT_TYPE_KEY_PRESSED, GameContainer::OnKeyEvent );
      this->core->Listen( this, Engine::EVENT_TYPE_KEY_RELEASED, GameContainer::OnKeyEvent );
      this->core->Listen( this, Engine::EVENT_TYPE_KEY_CLICKED, GameContainer::OnKeyEvent );
      this->core->Listen( this, Engine::EVENT_TYPE_KEY_DBLCLICKED, GameContainer::OnKeyEvent );
    }
    break;
    default: {
      LOGE( "GameContainer: unknown platform" );
    }
    break;
  }
  this->core->Listen( this, Engine::EVENT_TYPE_CORE_CLOSE, GameContainer::OnAppClose );
  this->core->Listen( this, Engine::EVENT_TYPE_RENDERER_INIT, GameContainer::OnGraphicsInit );

  //this->_TestFile();
  //this->_TestImageLoader();
  //this->_TestTexture();
  //this->_TestMesh();
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
  LOGI( "_TestFile... fileManager[%p]", this->core->GetFileManager() );
  if( this->core->GetFileManager()->GetFile( "qwe.tx", testmem, true ) ) {
    LOGI( "File finded: size[%d] content[%s]", ( int ) testmem.GetLength(), testmem.GetData() );
  } else {
    LOGE( "File not found" );
  }
}//_TestFile

void GameContainer::_TestImageLoader() {
  Engine::Memory imageFile;
  if( this->core->GetFileManager()->GetFile( "glow.tga", imageFile ) ) {
    Engine::ImageLoader loader;
    if( loader.Load( imageFile.GetData(), imageFile.GetLength() ) ) {
      LOGI( "loaded image: size[%dx%d] type[%d] transparent[%d]", ( int ) loader.imageWidth, ( int ) loader.imageHeight, ( int ) loader.imageType, ( int ) loader.isTransparent );
    } else {
      LOGE( "failed to load image" );
    }
  } else {
    LOGE( "_TestImageLoader => file not found" );
  }
}//_TestImageLoader

void GameContainer::_TestTexture() {
  Engine::Memory imageFile;
  if( this->core->GetFileManager()->GetFile( "glow.tga", imageFile ) ) {
    Engine::ImageLoader loader;
    if( loader.Load( imageFile.GetData(), imageFile.GetLength() ) ) {
      LOGI( "loaded image: size[%dx%d] type[%d] transparent[%d]", ( int ) loader.imageWidth, ( int ) loader.imageHeight, ( int ) loader.imageType, ( int ) loader.isTransparent );
      TextureType tex( loader.imageWidth, loader.imageHeight, loader.imageDataRGBA.GetData(), loader.isTransparent );
    } else {
      LOGE( "failed to load image" );
    }
  } else {
    LOGE( "_TestTexture => file not found" );
  }
}//_TestTexture

void GameContainer::_TestMesh() {
  static bool doInit = true;
  static Engine::Mesh *mesh = 0;
  static Engine::Material *material;
  static Engine::ShaderProgram *shader;
  static Engine::TextureType *texture0;
  static Engine::TextureType *texture1;
  if( doInit ) {
    LOGI( "init..." );
    doInit = false;
    shader = new Engine::ShaderProgram();
    Engine::Memory mem;

    game->core->GetFileManager()->GetFile( "test.vs", mem, true );
    shader->AttachVertexShader( ( char* ) mem.GetData(), mem.GetLength() );
    game->core->GetFileManager()->GetFile( "test.fs", mem, true );
    shader->AttachFragmentShader( ( char* ) mem.GetData(), mem.GetLength() );

    LOGI( "new texture" );
    Engine::Memory imageFile;
    game->core->GetFileManager()->GetFile( "cat.bmp", imageFile );
    Engine::ImageLoader loader;
    loader.Load( imageFile.GetData(), imageFile.GetLength() );
    texture0 = new Engine::TextureType( loader.imageWidth, loader.imageHeight, loader.imageDataRGBA.GetData(), loader.isTransparent );
    game->core->GetFileManager()->GetFile( "glow.tga", imageFile );
    loader.Load( imageFile.GetData(), imageFile.GetLength() );
    texture1 = new Engine::TextureType( loader.imageWidth, loader.imageHeight, loader.imageDataRGBA.GetData(), loader.isTransparent );
    LOGI( "new material..." );
    material = new Engine::Material( "test", shader );
    material->AddTexture( texture0 );
    material->AddTexture( texture1 );
    material->AddColor( Vec4( 1.0f, 0.0f, 0.0f, 0.5f ) );
    LOGI( "new mesh..." );
    mesh = new Engine::Mesh( game->core->renderer, material );
    LOGI( "new buffer..." );
    auto vertices = mesh->ResizeVertexBuffer( 6 );
    LOGI( "init buffer..." );
    vertices->Get( 0 ).pos.Set( 0.0f, 0.0f, 0.0f );
    vertices->Get( 0 ).tex.Set( 0.0f, 1.0f );
    vertices->Get( 1 ).pos.Set( 1.0f, 1.0f, 0.0f );
    vertices->Get( 1 ).tex.Set( 1.0f, 0.0f );
    vertices->Get( 2 ).pos.Set( 1.0f, 0.0f, 0.0f );
    vertices->Get( 2 ).tex.Set( 1.0f, 1.0f );

    vertices->Get( 3 ).pos.Set( 0.0f, 0.0f, 0.0f );
    vertices->Get( 3 ).tex.Set( 0.0f, 1.0f );
    vertices->Get( 4 ).pos.Set( 0.0f, 1.0f, 0.0f );
    vertices->Get( 4 ).tex.Set( 0.0f, 0.0f );
    vertices->Get( 5 ).pos.Set( 1.0f, 1.0f, 0.0f );
    vertices->Get( 5 ).tex.Set( 1.0f, 0.0f );
    LOGI( "ok" );
  } else {
    if( mesh ) {
      mesh->Render();
    }
  }
}//_TestMesh

void GameContainer::OnAfterRender( Engine::Listener* listener, Engine::Producer *producer, int eventId, void *data ) {
  LOGI( "OnAfterRender" );
  GameContainer *game = ( GameContainer* ) listener;
  game->_TestMesh();
}//OnAfterRender

void GameContainer::OnInitRender( Engine::Listener* listener, Engine::Producer *producer, int eventId, void *data ) {
  LOGI( "OnInitRender" );
}//OnInitRender
