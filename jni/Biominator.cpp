#include "Biominator.h"

using namespace Game;


GameContainer::GameContainer( Engine::Core *setCore )
:core( setCore ) {
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

  //this->_TestFile();
  //this->_TestImageLoader();
  //this->_TestTexture();
}

GameContainer::~GameContainer() {
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
  //GameContainer *game = ( GameContainer* ) listener;
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
  static Engine::TextureType *texture0;
  static Engine::TextureType *texture1;
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
    bool res;
    res = this->core->GetFileManager()->GetFile( "textures/glow.tga", imageFile );
    LOGI( "GetFile : %d", ( int ) res );
    Engine::ImageLoader loader;
    loader.Load( imageFile.GetData(), imageFile.GetLength() );
    texture0 = new Engine::TextureType( loader.imageWidth, loader.imageHeight, loader.imageDataRGBA.GetData(), loader.isTransparent, loader.isCompressed, loader.imageDataRGBA.GetLength(), loader.imageType );
    res = this->core->GetFileManager()->GetFile( "textures/cat.bmp", imageFile );
    LOGI( "GetFile : %d", ( int ) res );
    loader.Load( imageFile.GetData(), imageFile.GetLength() );
    texture1 = new Engine::TextureType( loader.imageWidth, loader.imageHeight, loader.imageDataRGBA.GetData(), loader.isTransparent, loader.isCompressed, loader.imageDataRGBA.GetLength(), loader.imageType );
    LOGI( "new material..." );
    material = new Engine::Material( "test", shader );
    material->AddTexture( texture0 );
    material->AddTexture( texture1 );
    material->AddColor( Vec4( 1.0f, 0.0f, 0.0f, 0.8f ) );
    LOGI( "new mesh..." );
    mesh = new Engine::Mesh( this->core->renderer, material );
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

    shader = this->CreateShader( "sprite-t", "shaders/sprite-t.vs", "shaders/sprite-t.fs" );
    material = this->CreateMaterial( "sprite-t", shader, "textures/cat.bmp" );
    Object *object = this->CreateSprite( "test", "sprite-t", Vec2( 1.0f, 1.0f ), Vec3( -0.5f, -0.5f, 0.0f ), Vec2( 1.0f, 0.5f ), 0.0f );

    Vec2 screenScale( 1.0f, 1.0f );
    bool isLandscape = false;
    if( this->core->renderer->GetScreenWidth() > this->core->renderer->GetScreenHeight() ) {
      screenScale.Set( ( ( float ) this->core->renderer->GetScreenHeight() ) / ( ( float ) this->core->renderer->GetScreenWidth() ) , 1.0f );
      isLandscape = true;
    } else {
      screenScale.Set( 1.0f, ( ( float ) this->core->renderer->GetScreenWidth() ) / ( ( float ) this->core->renderer->GetScreenHeight() ) );
      isLandscape = false;
    }
    LOGI( "scale: %3.3f x %3.3f", screenScale.x, screenScale.y );
    this->cameraMain.SetPosition( Vec3( 0.5f, 0.5f, 0.0f ) );
    this->cameraMain.SetScale( screenScale * 0.5f );
    this->cameraMain.SetRotation( 0.75f );
    this->cameraGUI.SetPosition( Vec3( 0.0f, 0.0f, 0.0f ) );
    this->cameraGUI.SetScale( Vec2( 1.0f, 1.0f ) );
    this->cameraGUI.SetRotation( 0.0f );

    shader = this->CreateShader( "sprite-tc", "shaders/sprite-tc.vs", "shaders/sprite-tc.fs" );
    material = this->CreateMaterial( "sprite-black", shader, "textures/blank.bmp" );
    material->AddColor( Vec4( 0.0f, 0.0f, 0.0f, 1.0f ) );
    if( isLandscape ) {
      float width = 1.0f - screenScale.x;
      this->CreateSprite( "/screen/border/left", "sprite-black", Vec2( width, 2.0f ), Vec3( -1.0f + width * 0.5f, 0.0f, -1.0f ), Vec2( 1.0f, 1.0f ), 0.0f )->SetWorldMatrix( this->cameraGUI.GetMatrixPointer() );
      this->CreateSprite( "/screen/border/right", "sprite-black", Vec2( width, 2.0f ), Vec3( 1.0f - width * 0.5f, 0.0f, -1.0f ), Vec2( 1.0f, 1.0f ), 0.0f )->SetWorldMatrix( this->cameraGUI.GetMatrixPointer() );
    } else {
      float height = 1.0f - screenScale.y;
      this->CreateSprite( "/screen/border/bottom", "sprite-black", Vec2( 2.0f, height ), Vec3( 0.0f, -1.0f + height * 0.5f, -1.0f ), Vec2( 1.0f, 1.0f ), 0.0f )->SetWorldMatrix( this->cameraGUI.GetMatrixPointer() );
      this->CreateSprite( "/screen/border/top", "sprite-black", Vec2( 2.0f, height ), Vec3( 0.0f, 1.0f - height * 0.5f, -1.0f ), Vec2( 1.0f, 1.0f ), 0.0f )->SetWorldMatrix( this->cameraGUI.GetMatrixPointer() );
    }

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
  game->cameraMain.Update();
  game->cameraGUI.Update();

  //test animation
  static float t = 0.0f;
  static Vec3 pos( Vec3Null ), posCenter( -0.5f, -0.5f, 0.0f );
  t += 0.01f;
  if( ( pos - posCenter ).LengthFast() < 0.02f ) {
    float
      a = ( ( float ) ( rand() % 1000 ) ) / 1000.0f * 3.141592f * 2.0f,
      r = 2.0f;
    pos.Set( Math::Sin16( a ) * r, Math::Cos16( a ) * r, 0.0f );
  }
  float speed = 0.02f;
  pos += ( posCenter - pos ) * speed;
  game->cameraMain.SetPosition( pos );
  //

  if( !game->objectsList.empty() ) {
    game->objectsList.begin()->second->SetRotation( Math::Sin16( t * 3.0f ) * 3.141592f );
  }

  game->_TestMesh();

  for( auto &object: game->objectsList ) {
    object.second->Render( game->cameraMain.GetMatrixPointer() );
  }
}//OnAfterRender


void GameContainer::OnInitRender( Engine::Listener* listener, Engine::Producer *producer, int eventId, void *data ) {
  LOGI( "OnInitRender" );
  GameContainer *game = ( GameContainer* ) listener;
  game->core->renderer->Listen( game, Engine::EVENT_TYPE_RENDERER_AFTER_RENDER, GameContainer::OnAfterRender );
  game->core->renderer->Listen( game, Engine::EVENT_TYPE_RENDERER_INIT, GameContainer::OnInitRender );
}//OnInitRender


Engine::Material* GameContainer::CreateMaterial( const std::string& name, Engine::ShaderProgram *shader, const std::string textureFileName ) {
  Engine::Memory imageFile;
  if( !this->core->GetFileManager()->GetFile( textureFileName, imageFile ) ) {
    LOGE( "GameContainer::CreateMaterial => image '%s' not found", textureFileName.c_str() );
    return NULL;
  }
  Engine::ImageLoader loader;
  if( !loader.Load( imageFile.GetData(), imageFile.GetLength() ) ) {
    LOGE( "GameContainer::CreateMaterial => can't load image '%s'", textureFileName.c_str() );
    return NULL;
  }
  Engine::Texture *texture = new Engine::TextureType( loader.imageWidth, loader.imageHeight, loader.imageDataRGBA.GetData(), loader.isTransparent, loader.isCompressed, loader.imageDataRGBA.GetLength(), loader.imageType );
  Engine::Material *material = this->CreateMaterial( name, shader );
  material->AddTexture( texture );

  return material;
}//CreateMaterial


Engine::Material* GameContainer::CreateMaterial( const std::string& name, Engine::ShaderProgram *shader ) {
  Engine::Material *material = new Engine::Material( name, shader );
  this->materialsList.insert( std::make_pair( name, material ) );

  return material;
}//CreateMaterial


Engine::Material* GameContainer::CreateMaterial( const std::string& name, Engine::ShaderProgram *shader, const std::string textureFileName, const Vec4& color ) {
  Engine::Material *material = this->CreateMaterial( name, shader, textureFileName );
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
:Mesh( renderer, material ), name( setName ), matrixChanged( true ), position( Vec3Null ), scale( Vec2One ), rotation( 0.0f ), size( Vec2One ) {
}


Object::~Object() {
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
    this->matrix = mTrans * mRot * mScale;
    this->objectMatrix = &this->matrix[ 0 ][ 0 ];
    LOGI( "calculated matrix: pos[%3.3f; %3.3f; %3.3f] scale[%3.3f; %3.3f] rot[%3.3f] matrix=>\n[%3.3f; %3.3f; %3.3f; %3.3f]\n[%3.3f; %3.3f; %3.3f; %3.3f]\n[%3.3f; %3.3f; %3.3f; %3.3f]\n[%3.3f; %3.3f; %3.3f; %3.3f]",
      this->position.x, this->position.y, this->position.z, this->scale.x, this->scale.y, this->rotation,
      this->matrix[ 0 ][ 0 ], this->matrix[ 0 ][ 1 ], this->matrix[ 0 ][ 2 ], this->matrix[ 0 ][ 3 ],
      this->matrix[ 1 ][ 0 ], this->matrix[ 1 ][ 1 ], this->matrix[ 1 ][ 2 ], this->matrix[ 1 ][ 3 ],
      this->matrix[ 2 ][ 0 ], this->matrix[ 2 ][ 1 ], this->matrix[ 2 ][ 2 ], this->matrix[ 2 ][ 3 ],
      this->matrix[ 3 ][ 0 ], this->matrix[ 3 ][ 1 ], this->matrix[ 3 ][ 2 ], this->matrix[ 3 ][ 3 ]
      );

    this->matrixChanged = false;
  }
}//BeforeRender
