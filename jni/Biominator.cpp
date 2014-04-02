#include "Biominator.h"
#include "time.h"
#include "stdlib.h"

using namespace Game;

Engine::PerObjectShaderBuffer* GameContainer::objectsMatricesList = NULL;


GameContainer::GameContainer( Engine::Core *setCore )
:Listener(), Producer(), core( setCore ) {
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
      this->core->Listen( this, Engine::EVENT_TYPE_CORE_CREATED_RENDERER, GameContainer::OnInitRender );
    }
    break;
    case Engine::PLATFORM_TYPE_LINUX: {
      this->core->Listen( this, Engine::EVENT_TYPE_KEY_PRESSED, GameContainer::OnKeyEvent );
      this->core->Listen( this, Engine::EVENT_TYPE_KEY_RELEASED, GameContainer::OnKeyEvent );
      this->core->Listen( this, Engine::EVENT_TYPE_KEY_CLICKED, GameContainer::OnKeyEvent );
      this->core->Listen( this, Engine::EVENT_TYPE_KEY_DBLCLICKED, GameContainer::OnKeyEvent );
      this->core->renderer->Listen( this, Engine::EVENT_TYPE_RENDERER_AFTER_RENDER, GameContainer::OnAfterRender );
      this->core->renderer->Listen( this, Engine::EVENT_TYPE_RENDERER_INIT, GameContainer::OnInitRender );
    }
    break;
    default: {
      LOGE( "GameContainer: unknown platform" );
    }
    break;
  }
  this->core->Listen( this, Engine::EVENT_TYPE_CORE_CLOSE, GameContainer::OnAppClose );
  this->core->Listen( this, Engine::EVENT_TYPE_RENDERER_INIT, GameContainer::OnGraphicsInit );
  this->objectsMatricesList = new Engine::PerObjectShaderBuffer();

  //this->_TestFile();
  //this->_TestImageLoader();
  //this->_TestTexture();
}

GameContainer::~GameContainer() {
  LOGI( "~GameContainer => remove objects" );
  for( auto &object: this->objectsList ) {
    SAFE_DELETE( object.second );
  }
  this->objectsList.clear();

  LOGI( "~GameContainer => remove matrices" );
  SAFE_DELETE( this->objectsMatricesList );

  //�� ������ ������ ������� ��������� ��������, �� ������ - ������
  LOGI( "~GameContainer => remove textures" );
  for( int pass = 0; pass < 2; ++pass ) {
    for( auto& texture: this->texturesList ) {
      if( texture.second && texture.second->IsAtlas() == ( pass == 1 ) ) {
        SAFE_DELETE( texture.second );
      }
    }
  }
  LOGI( "~GameContainer done" );
}

void GameContainer::OnKeyEvent( Engine::Listener* listener, Engine::Producer *producer, int eventId, void *data ) {
  Engine::EventKey *event = static_cast< Engine::EventKey* >( data );
  GameContainer *game = static_cast< GameContainer* >( listener );
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
  GameContainer *game = static_cast< GameContainer* >( listener );
  game->AppExit();
}//OnAppClose


//entry point
void Engine::EntryPoint::Init( Core* core ) {
  game = new GameContainer( core );
  if( core->renderer ) {
    core->renderer->Listen( game, Engine::EventType::EVENT_TYPE_RENDERER_BEFORE_RENDER, GameContainer::BeforeRender );
  }
}//Init

void Engine::EntryPoint::Destroy() {
  SAFE_DELETE( game );
}//Destroy

void GameContainer::OnGraphicsInit( Engine::Listener* listener, Engine::Producer *producer, int eventId, void *data ) {
}//OnGraphicsInit

void GameContainer::BeforeRender( Engine::Listener* listener, Engine::Producer *producer, int eventId, void *data ) {
  if( GameContainer::objectsMatricesList ) {
    GameContainer::objectsMatricesList->Update();
  }
  GameContainer *game = static_cast< GameContainer* >( listener );
  game->TouchEvent( Engine::EventType::EVENT_TYPE_USER_0, game );
}//BeforeRender

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
      //TextureType tex( loader.imageWidth, loader.imageHeight, loader.imageDataRGBA.GetData(), loader.isTransparent );
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
  static Engine::Texture *texture0;
  static Engine::Texture *texture1;
  if( doInit ) {
    LOGI( "init..." );
    doInit = false;
    LOGI( "shader..." );
    shader = new Engine::ShaderProgram();
    Engine::Memory mem;

    LOGI( "attach shaders..." );
    this->core->GetFileManager()->GetFile( "shaders/test.vs", mem, true );
    shader->AttachVertexShader( ( char* ) mem.GetData(), mem.GetLength() );
    this->core->GetFileManager()->GetFile( "shaders/test.fs", mem, true );
    shader->AttachFragmentShader( ( char* ) mem.GetData(), mem.GetLength() );

    LOGI( "new texture" );
    Engine::Memory imageFile;
    Engine::ImageLoader loader;
    texture0 = this->CreateTexture( "textures/glow.tga" );
    texture1 = this->CreateTexture( "textures/cat.bmp" );
    LOGI( "new material..." );
    material = new Engine::Material( "test", shader );
    material->AddTexture( texture0 );
    material->AddTexture( texture1 );
    material->AddColor( Vec4( 1.0f, 0.0f, 0.0f, 0.8f ) );
    this->materialsList.insert( std::make_pair( "test", material ) );

    shader = this->CreateShader( "sprite-t", "shaders/sprite-t.vs", "shaders/sprite-t.fs" );
    material = this->CreateMaterial( "sprite-t", shader, "textures/thu128.tga", Engine::TextureFilterType::TEXTURE_FILTER_TYPE_POINT );
    Object *object = this->CreateSprite( "test", "sprite-t", Vec2( 1.0f, 1.0f ), Vec3( -0.5f, -0.5f, 0.0f ), Vec2( 1.0f, 0.5f ), 0.0f );

    Vec2 screenScale( 1.0f, 1.0f );
    //bool isLandscape = false;
    if( this->core->renderer->GetScreenWidth() > this->core->renderer->GetScreenHeight() ) {
      screenScale.Set( ( ( float ) this->core->renderer->GetScreenHeight() ) / ( ( float ) this->core->renderer->GetScreenWidth() ) , 1.0f );
      //isLandscape = true;
    } else {
      screenScale.Set( 1.0f, ( ( float ) this->core->renderer->GetScreenWidth() ) / ( ( float ) this->core->renderer->GetScreenHeight() ) );
      //isLandscape = false;
    }
    LOGI( "scale: %3.3f x %3.3f", screenScale.x, screenScale.y );
    this->cameraMain.SetPosition( Vec3( 0.5f, 0.5f, 0.0f ) );
    this->cameraMain.SetScale( screenScale * 0.5f );
    this->cameraMain.SetRotation( 0.75f );
    this->cameraGUI.SetPosition( Vec3( 0.0f, 0.0f, 0.0f ) );
    this->cameraGUI.SetScale( Vec2( 1.0f, 1.0f ) );
    this->cameraGUI.SetRotation( 0.0f );
    this->camera3D.SetPosition( Vec3( 0.0f, 0.0f, 40.0f ) );
    this->camera3D.SetRotation( 0.0f, 0.0f, 0.0f );

    //this->camera3D
    shader = this->CreateShader( "mesh-3d", "shaders/mesh.vs", "shaders/mesh.fs" );
    material = this->CreateMaterial( "mesh-3d", shader, "textures/cat.bmp", Engine::TextureFilterType::TEXTURE_FILTER_TYPE_POINT );

    shader = this->CreateShader( "sprite-tc", "shaders/sprite-tc.vs", "shaders/sprite-tc.fs" );
    material = this->CreateMaterial( "sprite-black", shader, "textures/blank.bmp", Engine::TextureFilterType::TEXTURE_FILTER_TYPE_POINT );
    material->AddColor( Vec4( 0.0f, 0.0f, 0.0f, 1.0f ) );

    srand( ( unsigned int ) time( 0 ) );

    float
      borderWidth = 1.0f - screenScale.x,
      borderHeight = 1.0f - screenScale.y;
    this->CreateSprite( "/screen/border/left", "sprite-black", Vec2( borderWidth, 2.0f ), Vec3( -1.0f + borderWidth * 0.5f, 0.0f, -1.0f ), Vec2( 1.0f, 1.0f ), 0.0f )->SetWorldMatrix( this->cameraGUI.GetMatrixPointer() );
    this->CreateSprite( "/screen/border/right", "sprite-black", Vec2( borderWidth, 2.0f ), Vec3( 1.0f - borderWidth * 0.5f, 0.0f, -1.0f ), Vec2( 1.0f, 1.0f ), 0.0f )->SetWorldMatrix( this->cameraGUI.GetMatrixPointer() );
    this->CreateSprite( "/screen/border/bottom", "sprite-black", Vec2( 2.0f, borderHeight ), Vec3( 0.0f, -1.0f + borderHeight * 0.5f, -1.0f ), Vec2( 1.0f, 1.0f ), 0.0f )->SetWorldMatrix( this->cameraGUI.GetMatrixPointer() );
    this->CreateSprite( "/screen/border/top", "sprite-black", Vec2( 2.0f, borderHeight ), Vec3( 0.0f, 1.0f - borderHeight * 0.5f, -1.0f ), Vec2( 1.0f, 1.0f ), 0.0f )->SetWorldMatrix( this->cameraGUI.GetMatrixPointer() );

    if( true ) { //test 3d objects
      this->CreateSprite( "/test-3d-mesh", "mesh-3d", Vec2( 50.0f, 50.0f ), Vec3( 0.0f, 0.0f, 0.0f ), Vec2( 1.0f, 1.0f ), 0.0f )
        ->SetWorldMatrix( this->camera3D.GetWorldMatrixPointer() )
        ->SetProjectionMatrix( this->camera3D.GetProjectionMatrixPointer() );
      object = this->CreateObject( "/test/te123", "mesh-3d", Vec3( 0, 0, 0 ), Vec2One, 0.0f );
      object->SetWorldMatrix( this->camera3D.GetWorldMatrixPointer() );
      object->SetProjectionMatrix( this->camera3D.GetProjectionMatrixPointer() );
      auto buf = object->ResizeVertexBuffer( 15 );
      buf->Get( 0 ).pos.Set( -10, -10, -10 );
        buf->Get( 1 ).pos.Set(  10, -10, -10 );
        buf->Get( 2 ).pos.Set( 0, 10, 0 );
      buf->Get( 3 ).pos.Set(  10, -10, 10 );
        buf->Get( 4 ).pos.Set( -10, -10, 10 );
        buf->Get( 5 ).pos.Set( 0, 10, 0 );
      buf->Get( 6 ).pos.Set( -10, -10, -10 );
        buf->Get( 7 ).pos.Set( -10, -10, 10 );
        buf->Get( 8 ).pos.Set( 0, 10, 0 );
      buf->Get( 9 ).pos.Set( 10, -10, -10 );
        buf->Get( 10 ).pos.Set( 10, -10, 10 );
        buf->Get( 11 ).pos.Set( 0, 10, 0 );
      buf->Get( 12 ).pos.Set( 0, 0, 1 );
        buf->Get( 13 ).pos.Set( 10, 10, 1 );
        buf->Get( 14 ).pos.Set( 10, 0, 1 );
    }

    uint32_t atlasWidth = 512, atlasHeight = 256;
    Engine::Memory memAtlas( atlasWidth * atlasHeight * 4 );
    Engine::Texture* textureAtlas = this->CreateTexture( "atlas0", atlasWidth, atlasHeight, memAtlas.GetData(), true, false, memAtlas.GetLength() );
    textureAtlas->InitAtlas();
    textureAtlas->BindTextureToThisAtlas( this->GetTexture( "textures/cat.bmp" ) );
    textureAtlas->BindTextureToThisAtlas( this->GetTexture( "textures/thu128.tga" ) );

    //test static sprite
    this->CreateMaterial( "/sprite/textures/cat.bmp", this->GetShader( "sprite-tc" ) )
      ->AddTexture( this->GetTexture( "textures/cat.bmp" ) )
      ->AddColor( Vec4( 1.0f, 0.8f, 0.5f, 0.6f ) );
    this->CreateSprite( "/test/static/sprite", "/sprite/textures/cat.bmp", Vec2( 1.0f, 1.0f ), Vec3( 0.5f * screenScale.x, 0.5f * screenScale.y, 1.0f ), Vec2( 1.0f, 1.0f ), 0.0f )->SetWorldMatrix( this->cameraGUI.GetMatrixPointer() );
    /*
    LOGI( "new mesh..." );
    mesh = new Engine::Mesh( this->core->renderer, this->materialsList.find( "test" )->second );
    LOGI( "new buffer..." );
    auto vertices = mesh->ResizeVertexBuffer( 6 );
    LOGI( "init buffer..." );
    vertices->Get( 0 ).pos.Set( 0.0f, 0.0f, 1.0f );
    vertices->Get( 0 ).tex.Set( 0.0f, 1.0f );
    vertices->Get( 1 ).pos.Set( 1.0f, 1.0f, 1.0f );
    vertices->Get( 1 ).tex.Set( 1.0f, 0.0f );
    vertices->Get( 2 ).pos.Set( 1.0f, 0.0f, 1.0f );
    vertices->Get( 2 ).tex.Set( 1.0f, 1.0f );

    vertices->Get( 3 ).pos.Set( 0.0f, 0.0f, 1.0f );
    vertices->Get( 3 ).tex.Set( 0.0f, 1.0f );
    vertices->Get( 4 ).pos.Set( 0.0f, 1.0f, 1.0f );
    vertices->Get( 4 ).tex.Set( 0.0f, 0.0f );
    vertices->Get( 5 ).pos.Set( 1.0f, 1.0f, 1.0f );
    vertices->Get( 5 ).tex.Set( 1.0f, 0.0f );
    */
    textureAtlas->UnbindTextureFromThisAtlas( this->GetTexture( "textures/thu128.tga" ) );
    textureAtlas->BindTextureToThisAtlas( this->CreateTexture( "textures/alik16.bmp" ) );
    for( int q = 0; q < 100; ++q ) {
      textureAtlas->UnbindTextureFromThisAtlas( this->GetTexture( "textures/alik16.bmp" ) );
      textureAtlas->BindTextureToThisAtlas( this->GetTexture( "textures/alik16.bmp" ) );
    }
    textureAtlas->BindTextureToThisAtlas( this->GetTexture( "textures/thu128.tga" ) );
    this->CreateMaterial( "sprite-t-alik", this->GetShader( "sprite-t" ), "textures/alik16.bmp", Engine::TextureFilterType::TEXTURE_FILTER_TYPE_POINT );
    this->CreateSprite( "test/alik", "sprite-t-alik", Vec2( 0.5f, 0.5f ), Vec3( -0.5f, 0.5f, -1.0f ), Vec2( 2.0f, 1.0f ), 0.4f )->SetWorldMatrix( this->cameraMain.GetMatrixPointer() );

    this->CreateShader( "sprite-bacterium", "shaders/sprite-bacterium.vs", "shaders/sprite-bacterium.fs" );
    this->CreateTexture( "textures/bioma-skin0.tga", Engine::TextureFilterType::TEXTURE_FILTER_TYPE_LINEAR );
    this->CreateMaterial( "bacterium-0", this->GetShader( "sprite-bacterium" ), "textures/bioma-skin0.tga", Vec4( 1.0f, 1.0f, 1.0f, 0.98f ), Engine::TextureFilterType::TEXTURE_FILTER_TYPE_LINEAR );
    this->CreateBacterium( "bac0", "bacterium-0", Vec2( 1.0f, 1.0f ), Vec3( 0.0f, -1.0f, 0.0f ), Vec2( 1.0f, 1.0f ), 0.0f, 5 )->SetWorldMatrix( this->cameraMain.GetMatrixPointer() );

    LOGI( "ok" );
  } else {
    if( mesh ) {
      mesh->Render();
    }
  }
}//_TestMesh


void GameContainer::OnAfterRender( Engine::Listener* listener, Engine::Producer *producer, int eventId, void *data ) {
  GameContainer *game = static_cast< GameContainer* >( listener );
  game->cameraMain.Update();
  game->cameraGUI.Update();
  game->camera3D.Update();
  float f = game->core->timer->GetDeltaF();

  //test animation
  static float t = 0.0f;
  static Vec3 pos( Vec3Null ), posCenter( -0.5f, -0.5f, 0.0f );
  t += 0.5f * f;
  if( ( pos - posCenter ).Length() < 0.02f ) {
    float
      a = ( ( float ) ( rand() % 1000 ) ) / 1000.0f * 3.141592f * 2.0f,
      r = 2.0f;
    pos.Set( Math::Sin16( a ) * r, Math::Cos16( a ) * r, 0.0f );
  }
  float speed = 0.02f;
  pos += ( posCenter - pos ) * speed;
  game->cameraMain.SetPosition( pos );
  //

  game->camera3D.SetPosition( Vec3( Math::Sin16( t * 3.0f ) * 20.0f, 0.0f, -50.0f ) );
  if( game->objectsList.find( "/test-3d-mesh" ) != game->objectsList.end() ) {
    game->objectsList.find( "/test-3d-mesh" )->second->SetRotation( t * 4.0f );
  }
  game->camera3D.SetRotation( 0, Math::Sin( t * 2.0f ) * 180.0f, Math::Cos( t * 3.0f ) * 180.0f );
  //game->camera3D.SetRotation( 0, 180.0f, 180.0f );
  //game->camera3D.SetRotation( t * 100.0f, 0.0f, 0.0f );
  //game->camera3D.SetRotation( 0.0f, Math::Sin16( t ) * 30.0f, Math::Cos16( t ) * 30.0f );

  if( game->objectsList.find( "test" ) != game->objectsList.end() ) {
    game->objectsList.find( "test" )->second->SetRotation( Math::Sin16( t * 3.0f ) * 3.141592f );
  }

  if( game->objectsList.find( "bac0" ) != game->objectsList.end() ) {
    game->objectsList.find( "bac0" )->second->_Test();
  }

  game->_TestMesh();

  for( auto &object: game->objectsList ) {
    object.second->Render( game->cameraMain.GetMatrixPointer() );
  }
}//OnAfterRender


void GameContainer::OnInitRender( Engine::Listener* listener, Engine::Producer *producer, int eventId, void *data ) {
  LOGI( "OnInitRender" );
  GameContainer *game = static_cast< GameContainer* >( listener );
  game->core->renderer->Listen( game, Engine::EVENT_TYPE_RENDERER_AFTER_RENDER, GameContainer::OnAfterRender );
  game->core->renderer->Listen( game, Engine::EVENT_TYPE_RENDERER_INIT, GameContainer::OnInitRender );
}//OnInitRender


Engine::Material* GameContainer::CreateMaterial( const std::string& name, Engine::ShaderProgram *shader, const std::string textureFileName, Engine::TextureFilterType filter ) {
  Engine::Texture *texture = this->GetTexture( textureFileName, true );
  if( !texture ) {
    texture = this->CreateTexture( textureFileName, filter );
  }

  Engine::Material *material = this->CreateMaterial( name, shader );
  material->AddTexture( texture );

  return material;
}//CreateMaterial


Engine::Material* GameContainer::CreateMaterial( const std::string& name, Engine::ShaderProgram *shader ) {
  Engine::Material *material = new Engine::Material( name, shader );
  this->materialsList.insert( std::make_pair( name, material ) );

  return material;
}//CreateMaterial


Engine::Material* GameContainer::CreateMaterial( const std::string& name, Engine::ShaderProgram *shader, const std::string textureFileName, const Vec4& color, Engine::TextureFilterType filter ) {
  Engine::Material *material = this->CreateMaterial( name, shader, textureFileName, filter );
  if( !material ) {
    return NULL;
  }
  material->AddColor( color );

  return material;
}//CreateMaterial


Engine::ShaderProgram* GameContainer::CreateShader( const std::string& name, const std::string& vertexShaderFileName, const std::string& fragmentShaderFileName ) {
  Engine::ShaderProgram *shader = new Engine::ShaderProgram;
  Engine::Memory mem;
  if( !this->core->GetFileManager()->GetFile( vertexShaderFileName, mem, true ) ) {
    LOGE( "GameContainer::CreateShader => vertex shader not found" );
    SAFE_DELETE( shader );
    return NULL;
  }
  shader->AttachVertexShader( ( char* ) mem.GetData(), mem.GetLength() );
  if( !this->core->GetFileManager()->GetFile( fragmentShaderFileName, mem, true ) ) {
    LOGE( "GameContainer::CreateShader => fragment shader not found" );
    SAFE_DELETE( shader );
    return NULL;
  }
  shader->AttachFragmentShader( ( char* ) mem.GetData(), mem.GetLength() );
  this->shadersList.insert( std::make_pair( name, shader ) );

  return shader;
}//CreateShader


Object* GameContainer::CreateObject( const std::string &name, const std::string &materialName, const Vec3& position, const Vec2& scale, const float rotation ) {
  auto iterMaterial = this->materialsList.find( materialName );
  if( iterMaterial == this->materialsList.end() ) {
    LOGE( "GameContainer::CreateObject => material '%s' not found", materialName.c_str() );
    return NULL;
  }
  Object *obj = new Object( name, this->core->renderer, iterMaterial->second );
  this->objectsList.insert( std::make_pair( name, obj ) );
  obj->SetPosition( position );
  obj->SetScale( scale );
  obj->SetRotation( rotation );

  return obj;
}//CreateObject


Object* GameContainer::CreateSprite( const std::string &name, const std::string &materialName, const Vec2& size, const Vec3& position, const Vec2& scale, const float rotation ) {
  Object *object = this->CreateObject( name, materialName, position, scale, rotation );
  if( !object ) {
    LOGE( "GameContainer::CreateSprite => object is NULL" );
    return NULL;
  }

  Vec2 boxSize( 0.5f, 0.5f );
  Engine::VertexBufferWriter *vertices = object->ResizeVertexBuffer( 6 );
  vertices->Get( 0 ).pos.Set( -boxSize.x, -boxSize.y, 0.0f );
  vertices->Get( 0 ).tex.Set( 0.0f, 1.0f );
  vertices->Get( 1 ).pos.Set( boxSize.x, boxSize.y, 0.0f );
  vertices->Get( 1 ).tex.Set( 1.0f, 0.0f );
  vertices->Get( 2 ).pos.Set( boxSize.x, -boxSize.y, 0.0f );
  vertices->Get( 2 ).tex.Set( 1.0f, 1.0f );

  vertices->Get( 3 ).pos.Set( -boxSize.x, -boxSize.y, 0.0f );
  vertices->Get( 3 ).tex.Set( 0.0f, 1.0f );
  vertices->Get( 4 ).pos.Set( -boxSize.x, boxSize.y, 0.0f );
  vertices->Get( 4 ).tex.Set( 0.0f, 0.0f );
  vertices->Get( 5 ).pos.Set( boxSize.x, boxSize.y, 0.0f );
  vertices->Get( 5 ).tex.Set( 1.0f, 0.0f );

  object->SetSize( size );

  return object;
}//CreateSprite


void GameContainer::DeleteObject( Object *obj ) {
  if( !obj ) {
    return;
  }
  this->DeleteObject( obj->GetName() );
}//DeleteObject


void GameContainer::DeleteObject( const std::string& objectName ) {
  auto iterObj = this->objectsList.find( objectName );
  if( iterObj == this->objectsList.end() ) {
    return;
  }
  SAFE_DELETE( iterObj->second );
  this->objectsList.erase( iterObj );
}//DeleteObject




Object::Object( const std::string &setName, Engine::Renderer *renderer, Engine::Material *material )
:Mesh( renderer, material ), name( setName ), position( Vec3Null ), scale( Vec2One ), size( Vec2One ), rotation( 0.0f ), matrixChanged( true ) {
  if( !GameContainer::objectsMatricesList ) {
    LOGE( "GameContainer::objectsMatricesList is null" );
  }
  this->objectMatrixIndex = GameContainer::objectsMatricesList->AddContainer();
  LOGI( "objectMatrixIndex => %d", this->objectMatrixIndex );
}


Object::~Object() {
  LOGI( "~Object %p", this );
  GameContainer::objectsMatricesList->DeleteContainer( this->objectMatrixIndex );
  LOGI( "~Object %p done", this );
}


Object* Object::SetPosition( const Vec3& setPosition ) {
  if( this->position.x != setPosition.x || this->position.y != setPosition.y || this->position.z != setPosition.z ) {
    this->matrixChanged = true;
  }
  this->position = setPosition;
  return this;
}//SetPosition


Object* Object::SetScale( const Vec2& setScale ) {
  if( this->scale.x != setScale.x || this->scale.y != setScale.y ) {
    this->matrixChanged = true;
  }
  this->scale = setScale;
  return this;
}//SetScale


Object* Object::SetSize( const Vec2& setSize ) {
  if( this->size.x != setSize.x || this->size.y != setSize.y ) {
    this->matrixChanged = true;
  }
  this->size = setSize;
  return this;
}//SetSize


Object* Object::SetRotation( const float setRotation ) {
  if( this->rotation != setRotation ) {
    this->matrixChanged = true;
  }
  this->rotation = setRotation;
  return this;
}//SetRotation


void Object::BeforeRender() {
  if( this->matrixChanged ) {
    Mat4 mRot, mTrans, mScale;
    mTrans.Identity();
    mRot.Identity();
    mScale.Identity();

    mTrans[ 0 ][ 3 ] = this->position.x;
    mTrans[ 1 ][ 3 ] = this->position.y;
    mTrans[ 2 ][ 3 ] = this->position.z;

    float cosa = Math::Cos16( this->rotation );
    float sina = Math::Sin16( this->rotation );
    mRot[ 0 ][ 0 ] = cosa;
    mRot[ 1 ][ 0 ] = -sina;
    mRot[ 0 ][ 1 ] = sina;
    mRot[ 1 ][ 1 ] = cosa;
    mScale[ 0 ][ 0 ] = this->scale.x * this->size.x;
    mScale[ 1 ][ 1 ] = this->scale.y * this->size.y;

    this->matrix = mTrans * mRot * mScale;  //src

    this->objectMatrix = &this->matrix[ 0 ][ 0 ];
    Engine::PerObjectShaderBuffer::Container *matrixContainer = GameContainer::objectsMatricesList->GetContainer( this->objectMatrixIndex );
    if( matrixContainer ) {
      matrixContainer->modelMatrix = this->matrix;
      matrixContainer->TexCoordsOffset = this->material->GetTextureCoordsOffset();
      matrixContainer->TexCoordsScale = this->material->GetTextureCoordsScale();
      auto verticesCount = this->vertices->GetSize();
      for( size_t verticeNum = 0; verticeNum < verticesCount; ++verticeNum ) {
        Engine::Vertice &vertice = this->vertices->Get( verticeNum );
        vertice.modelMatrix = this->matrix;
        vertice.texCoordsScale = this->material->GetTextureCoordsScale();
        vertice.texCoordsOffset = this->material->GetTextureCoordsOffset();
      }
    } else {
      LOGE( "Object::BeforeRender => matrix container not found" );
    }

    this->matrixChanged = false;
  }

  //GameContainer::objectsMatricesList->BindToShader( *this->material->shaderProgram, this->objectMatrixIndex );
}//BeforeRender

Engine::Texture* GameContainer::CreateTexture( const std::string &name, size_t setWidth, size_t setHeight, unsigned char *data, bool setIsTransparent, bool setIsCompressed, size_t setDataLength, Engine::ImageType setImageFormat, Engine::TextureFilterType filter ) {
  Engine::Texture *texture = new Engine::TextureType( setWidth, setHeight, data, setIsTransparent, setIsCompressed, setDataLength, setImageFormat, filter );
  this->texturesList.insert( std::make_pair( name, texture ) );

  return texture;
}//CreateTexture

Engine::Texture* GameContainer::CreateTexture( const std::string &fileName, Engine::TextureFilterType filter ) {
  Engine::Memory imageFile;
  if( !this->core->GetFileManager()->GetFile( fileName, imageFile ) ) {
    LOGE( "GameContainer::CreateTexture => file '%s' not found", fileName.c_str() );
    return NULL;
  }
  Engine::ImageLoader loader;
  loader.Load( imageFile.GetData(), imageFile.GetLength() );
  return this->CreateTexture( fileName, loader.imageWidth, loader.imageHeight, loader.imageDataRGBA.GetData(), loader.isTransparent, loader.isCompressed, loader.imageDataRGBA.GetLength(), loader.imageType, filter );
}//CreateTexture

Engine::Texture* GameContainer::GetTexture( const std::string &name, bool supressWarning ) {
  auto texture = this->texturesList.find( name );
  if( texture == this->texturesList.end() ) {
    if( !supressWarning ) {
      LOGE( "GameContainer::GetTexture => texture '%s' not found", name.c_str() );
    }
    return NULL;
  }

  return texture->second;
}//GetTexture

Engine::ShaderProgram* GameContainer::GetShader( const std::string& name, bool supressWarning ) {
  auto shader = this->shadersList.find( name );
  if( shader == this->shadersList.end() ) {
    if( !supressWarning ) {
      LOGE( "GameContainer::GetShader => shader '%s' not found", name.c_str() );
    }
    return NULL;
  }

  return shader->second;
}//GetShader


Engine::Material* GameContainer::GetMaterial( const std::string& name ) {
  auto material = this->materialsList.find( name );
  if( material == this->materialsList.end() ) {
    LOGE( "GameContainer::GetMaterial => material '%s' not found", name.c_str() );
    return NULL;
  }

  return material->second;
}//GetMaterial


Object* GameContainer::CreateBacterium( const std::string &name, const std::string &materialName, const Vec2& size, const Vec3& position, const Vec2& scale, const float rotation, const int bonesCount ) {
  ObjectBacterium *object = new ObjectBacterium( name, this->core->renderer, this->GetMaterial( materialName ), 1.0f, bonesCount );
  object->SetPosition( position );
  object->SetRotation( rotation );
  object->SetScale( scale );
  this->Listen( object, Engine::EventType::EVENT_TYPE_USER_0, ObjectBacterium::Update );
  this->objectsList.insert( std::make_pair( name, object ) );
  return object;
}//CreateBacterium


ObjectBacterium::ObjectBacterium( const std::string &setName, Engine::Renderer *renderer, Engine::Material *material, const float radius, const int bonesCount )
:Object( setName, renderer, material ), Listener() {
  const int polygonsCount = 64;
  const int verticesBaseCount = polygonsCount * 3;
  const int verticesCount = verticesBaseCount + 4 * 3;
  auto buffer = this->ResizeVertexBuffer( verticesCount );
  const float deltaRadius = 0.15f;
  const float legs = float( bonesCount );
  int num;
  Engine::Vertice *vertice;
  for( num = 0; num < polygonsCount; ++num ) {
    float
      deg = float( num ) / float( polygonsCount ) * Math::TWO_PI,
      nextDeg = float( num + 1 ) / float( polygonsCount ) * Math::TWO_PI,
      newRadius = radius * ( 1.0f + Math::Cos16( deg * legs ) * deltaRadius ),
      newNextRadius = radius * ( 1.0f + Math::Cos16( nextDeg * legs ) * deltaRadius );

    vertice = &buffer->Get( num * 3 + 0 );
    vertice->pos.Set( Math::Sin16( deg ) * newRadius, Math::Cos16( deg ) * newRadius, 0.0f );
    vertice->color.Set( 1.0f, 1.0f, 1.0f, 1.0f );
    vertice->tex.Set( 0.0f, 0.0f );
    vertice->texCoordsOffset.Set( 0.0f, 0.0f );
    vertice->texCoordsScale.Set( 1.0f, 1.0f );

    vertice = &buffer->Get( num * 3 + 1 );
    vertice->pos.Set( Math::Sin16( nextDeg ) * newNextRadius, Math::Cos16( nextDeg ) * newNextRadius, 0.0f );
    vertice->color.Set( 1.0f, 1.0f, 1.0f, 1.0f );
    vertice->tex.Set( 0.0f, 0.0f );
    vertice->texCoordsOffset.Set( 0.0f, 0.0f );
    vertice->texCoordsScale.Set( 1.0f, 1.0f );

    vertice = &buffer->Get( num * 3 + 2 );
    vertice->pos.Set( 0.0f, 0.0f, 0.0f );
    vertice->color.Set( 1.0f, 1.0f, 1.0f, 1.0f );
    vertice->tex.Set( 0.0f, 72.0f / 128.0f );
    vertice->texCoordsOffset.Set( 0.0f, 0.0f );
    vertice->texCoordsScale.Set( 1.0f, 1.0f );
  }


  for( int num = 0; num < bonesCount; ++num ) {
    float
      deg = float( num ) / float( bonesCount ) * Math::TWO_PI,
      boneLength = radius * ( 0.8f + float( rand() % 1000 ) * 0.0004f );
    ObjectBacteriumBone *bone = new ObjectBacteriumBone();
    bone->basePosition.Set( Math::Sin16( deg ) * boneLength, Math::Cos16( deg ) * boneLength, 0.0f );
    bone->position = bone->basePosition;
    bone->tension = float( rand() % 1000 ) * 0.001f * 0.5f + 0.5f;
    this->bones.push_back( bone );
    
    for( int verticeNum = 0; verticeNum < verticesBaseCount; ++verticeNum ) {
      float distance = ( this->vertices->Get( verticeNum ).pos - bone->basePosition ).Length();
      if( distance > radius || this->vertices->Get( verticeNum ).pos.Length() < 0.1f ) {
        continue;
      }
      float morphingPower = ( distance < Math::FLT_EPSILON_NUM ? 1.0f : Min2( 1.0f / ( distance * distance ) / ( 2.0f * radius ), 1.0f ) );
      bone->vertices.push_back( ObjectBacteriumBone::VerticeInfo( verticeNum, morphingPower, this->vertices->Get( verticeNum ).pos ) );
      //LOGI( "Bone %d: pos[%3.3f; %3.3f] distance[%3.3f] tension[%3.3f] vert[%d] morphPower[%3.3f]", num, bone->basePosition.x, bone->basePosition.y, distance, bone->tension, verticeNum, morphingPower );
    }
  }

  //defaultVerticesPosition
  for( int verticeNum = 0; verticeNum < verticesBaseCount; ++verticeNum ) {
    this->defaultVerticesPosition.push_back( ObjectBacteriumBone::VerticeInfo( verticeNum, 0.0f, this->vertices->Get( verticeNum ).pos ) );
  }

  //left eye
  float
    eyesRange = 0.3f * radius,
    eyesSize = 0.15f * radius;
  num = verticesBaseCount - 1;
  Vec4 eyeColor( 0.7f, 0.9f, 1.0f, 0.8f );

  vertice = &buffer->Get( ++num );
  vertice->pos = Vec3( -1.0f, 1.0f, 0.0f ) * radius * eyesRange + Vec3( -1.0f, 1.0f, 0.0f ) * eyesSize;
  vertice->color = eyeColor;
  vertice->tex.Set( 2.0f / 32.0f, 0.0f );
  vertice->texCoordsOffset.Set( 0.0f, 0.0f );
  vertice->texCoordsScale.Set( 1.0f, 1.0f );
  this->defaultVerticesPosition.push_back( ObjectBacteriumBone::VerticeInfo( num, 0.0f, vertice->pos ) );

  vertice = &buffer->Get( ++num );
  vertice->pos = Vec3( -1.0f, 1.0f, 0.0f ) * radius * eyesRange + Vec3( 1.0f, 1.0f, 0.0f ) * eyesSize;
  vertice->color = eyeColor;
  vertice->tex.Set( 18.0f / 32.0f, 0.0f );
  vertice->texCoordsOffset.Set( 0.0f, 0.0f );
  vertice->texCoordsScale.Set( 1.0f, 1.0f );
  this->defaultVerticesPosition.push_back( ObjectBacteriumBone::VerticeInfo( num, 0.0f, vertice->pos ) );

  vertice = &buffer->Get( ++num );
  vertice->pos = Vec3( -1.0f, 1.0f, 0.0f ) * radius * eyesRange + Vec3( -1.0f, -1.0f, 0.0f ) * eyesSize;
  vertice->color = eyeColor;
  vertice->tex.Set( 2.0f / 32.0f, 16.0f / 128.0f );
  vertice->texCoordsOffset.Set( 0.0f, 0.0f );
  vertice->texCoordsScale.Set( 1.0f, 1.0f );
  this->defaultVerticesPosition.push_back( ObjectBacteriumBone::VerticeInfo( num, 0.0f, vertice->pos ) );

  vertice = &buffer->Get( ++num );
  vertice->pos = Vec3( -1.0f, 1.0f, 0.0f ) * radius * eyesRange + Vec3( -1.0f, -1.0f, 0.0f ) * eyesSize;
  vertice->color = eyeColor;
  vertice->tex.Set( 2.0f / 32.0f, 16.0f / 128.0f );
  vertice->texCoordsOffset.Set( 0.0f, 0.0f );
  vertice->texCoordsScale.Set( 1.0f, 1.0f );
  this->defaultVerticesPosition.push_back( ObjectBacteriumBone::VerticeInfo( num, 0.0f, vertice->pos ) );

  vertice = &buffer->Get( ++num );
  vertice->pos = Vec3( -1.0f, 1.0f, 0.0f ) * radius * eyesRange + Vec3( 1.0f, 1.0f, 0.0f ) * eyesSize;
  vertice->color = eyeColor;
  vertice->tex.Set( 18.0f / 32.0f, 0.0f );
  vertice->texCoordsOffset.Set( 0.0f, 0.0f );
  vertice->texCoordsScale.Set( 1.0f, 1.0f );
  this->defaultVerticesPosition.push_back( ObjectBacteriumBone::VerticeInfo( num, 0.0f, vertice->pos ) );

  vertice = &buffer->Get( ++num );
  vertice->pos = Vec3( -1.0f, 1.0f, 0.0f ) * radius * eyesRange + Vec3( 1.0f, -1.0f, 0.0f ) * eyesSize;
  vertice->color = eyeColor;
  vertice->tex.Set( 18.0f / 32.0f, 16.0f / 128.0f );
  vertice->texCoordsOffset.Set( 0.0f, 0.0f );
  vertice->texCoordsScale.Set( 1.0f, 1.0f );
  this->defaultVerticesPosition.push_back( ObjectBacteriumBone::VerticeInfo( num, 0.0f, vertice->pos ) );

  //right eye

  vertice = &buffer->Get( ++num );
  vertice->pos = Vec3( 1.0f, 1.0f, 0.0f ) * radius * eyesRange + Vec3( -1.0f, 1.0f, 0.0f ) * eyesSize;
  vertice->color = eyeColor;
  vertice->tex.Set( 18.0f / 32.0f, 0.0f );
  vertice->texCoordsOffset.Set( 0.0f, 0.0f );
  vertice->texCoordsScale.Set( 1.0f, 1.0f );
  this->defaultVerticesPosition.push_back( ObjectBacteriumBone::VerticeInfo( num, 0.0f, vertice->pos ) );

  vertice = &buffer->Get( ++num );
  vertice->pos = Vec3( 1.0f, 1.0f, 0.0f ) * radius * eyesRange + Vec3( 1.0f, 1.0f, 0.0f ) * eyesSize;
  vertice->color = eyeColor;
  vertice->tex.Set( 2.0f / 32.0f, 0.0f );
  vertice->texCoordsOffset.Set( 0.0f, 0.0f );
  vertice->texCoordsScale.Set( 1.0f, 1.0f );
  this->defaultVerticesPosition.push_back( ObjectBacteriumBone::VerticeInfo( num, 0.0f, vertice->pos ) );

  vertice = &buffer->Get( ++num );
  vertice->pos = Vec3( 1.0f, 1.0f, 0.0f ) * radius * eyesRange + Vec3( -1.0f, -1.0f, 0.0f ) * eyesSize;
  vertice->color = eyeColor;
  vertice->tex.Set( 18.0f / 32.0f, 16.0f / 128.0f );
  vertice->texCoordsOffset.Set( 0.0f, 0.0f );
  vertice->texCoordsScale.Set( 1.0f, 1.0f );
  this->defaultVerticesPosition.push_back( ObjectBacteriumBone::VerticeInfo( num, 0.0f, vertice->pos ) );

  vertice = &buffer->Get( ++num );
  vertice->pos = Vec3( 1.0f, 1.0f, 0.0f ) * radius * eyesRange + Vec3( -1.0f, -1.0f, 0.0f ) * eyesSize;
  vertice->color = eyeColor;
  vertice->tex.Set( 18.0f / 32.0f, 16.0f / 128.0f );
  vertice->texCoordsOffset.Set( 0.0f, 0.0f );
  vertice->texCoordsScale.Set( 1.0f, 1.0f );
  this->defaultVerticesPosition.push_back( ObjectBacteriumBone::VerticeInfo( num, 0.0f, vertice->pos ) );

  vertice = &buffer->Get( ++num );
  vertice->pos = Vec3( 1.0f, 1.0f, 0.0f ) * radius * eyesRange + Vec3( 1.0f, 1.0f, 0.0f ) * eyesSize;
  vertice->color = eyeColor;
  vertice->tex.Set( 2.0f / 32.0f, 0.0f );
  vertice->texCoordsOffset.Set( 0.0f, 0.0f );
  vertice->texCoordsScale.Set( 1.0f, 1.0f );
  this->defaultVerticesPosition.push_back( ObjectBacteriumBone::VerticeInfo( num, 0.0f, vertice->pos ) );

  vertice = &buffer->Get( ++num );
  vertice->pos = Vec3( 1.0f, 1.0f, 0.0f ) * radius * eyesRange + Vec3( 1.0f, -1.0f, 0.0f ) * eyesSize;
  vertice->color = eyeColor;
  vertice->tex.Set( 2.0f / 32.0f, 16.0f / 128.0f );
  vertice->texCoordsOffset.Set( 0.0f, 0.0f );
  vertice->texCoordsScale.Set( 1.0f, 1.0f );
  this->defaultVerticesPosition.push_back( ObjectBacteriumBone::VerticeInfo( num, 0.0f, vertice->pos ) );

  /*
  vertice = &buffer->Get( num++ * 3 + 0 );
  vertice->pos = Vec3( -1.0f, 1.0f, 0.0f ) * radius * 0.5f + Vec3( 0.0f, -radius * 0.3f, 0.0f );
  vertice->color.Set( 1.0f, 1.0f, 1.0f, 1.0f );
  vertice->tex.Set( 0.0f, 0.125f );
  vertice->texCoordsOffset.Set( 0.0f, 0.0f );
  vertice->texCoordsScale.Set( 1.0f, 1.0f );

  vertice = &buffer->Get( num++ * 3 + 0 );
  vertice->pos = Vec3( -1.0f, 1.0f, 0.0f ) * radius * 0.5f + Vec3( radius * 0.3f, 0.0f, 0.0f );
  vertice->color.Set( 1.0f, 1.0f, 1.0f, 1.0f );
  vertice->tex.Set( 0.5f, 0.0f );
  vertice->texCoordsOffset.Set( 0.0f, 0.0f );
  vertice->texCoordsScale.Set( 1.0f, 1.0f );

  vertice = &buffer->Get( num++ * 3 + 0 );
  vertice->pos = Vec3( -1.0f, 1.0f, 0.0f ) * radius * 0.5f + Vec3( radius * 0.3f, -radius * 0.3f, 0.0f );
  vertice->color.Set( 1.0f, 1.0f, 1.0f, 1.0f );
  vertice->tex.Set( 0.5f, 0.125f );
  vertice->texCoordsOffset.Set( 0.0f, 0.0f );
  vertice->texCoordsScale.Set( 1.0f, 1.0f );
  */

  this->rotationSpeed = ( float( rand() % 1000 ) / 500.0f - 1.0f ) * 0.01f;

  LOGI( "ObjectBacterium %p", this );
}


ObjectBacterium::~ObjectBacterium() {
  LOGI( "~ObjectBacterium %p", this );
}


ObjectBacteriumBone::ObjectBacteriumBone() {
}

ObjectBacteriumBone::ObjectBacteriumBone( const ObjectBacteriumBone& bone )
:basePosition( bone.position ), position( bone.position ), tension( bone.tension ) {
  this->vertices = bone.vertices;
}

ObjectBacteriumBone& ObjectBacteriumBone::operator=( ObjectBacteriumBone& bone ) {
  this->basePosition = bone.basePosition;
  this->position = bone.position;
  this->tension = bone.tension;
  this->vertices = bone.vertices;

  return *this;
}

void ObjectBacterium::Update( Engine::Listener* listener, Engine::Producer *producer, int eventId, void *data ) {
  ObjectBacterium *object = static_cast< ObjectBacterium* >( listener );

  object->RecalculateBones();
}//Update


void ObjectBacterium::RecalculateBones() {
  for( auto& verticeInfo: this->defaultVerticesPosition ) {
    this->vertices->Get( verticeInfo.index ).pos = verticeInfo.srcPosition;
  }

  for( auto& bone: this->bones ) {
    for( auto& verticeInfo: bone->vertices ) {
      Engine::Vertice &vertice = this->vertices->Get( verticeInfo.index );
      Vec3 boneDeltaPos = bone->position - bone->basePosition;
      Vec3 deltaPos = boneDeltaPos * verticeInfo.morphPower;
      vertice.pos += deltaPos;
    }
  }
}//RecalculateBones

void ObjectBacterium::_Test() {
  float t = float( game->core->timer->GetTime() ) * 3.0f;
  float r = 0.1f;
  int num = 0;
  float angleBetweenBones = Math::TWO_PI / float( this->bones.size() );
  for( auto& bone: this->bones ) {
    float dt = float( num );
    float sign = ( num % 2 ? -1.0f : 1.0f );
    float direction = Math::Sin16( float( num ) * angleBetweenBones * 1.5f + 0.5f );
    Vec3 pos( Math::Sin16( t * sign + dt ) * r * direction, Math::Cos16( t * sign + dt ) * r * direction, 0.0f );
    bone->position = bone->basePosition + pos;
    ++num;
  }

  this->SetRotation( this->GetRotation() + this->rotationSpeed );
}//_Test
