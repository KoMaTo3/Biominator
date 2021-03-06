#pragma once

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
/*
#include "memory.h"
#include "vertexbuffergl.h"
#include "platform/texture.h"
*/
#include "material.h"
#include "shaderprogram.h"
#include "mesh.h"
#include "imageloader.h"
#include "filemanager.h"
#include "renderer.h"
#include "platform/texture.h"
#include "types.h"
#include "math/kmatrix.h"
#include "camera2d.h"
#include "camera3d.h"
#include "perobjectshaderbuffer.h"


namespace Game {

class KeyListener;
class Object;

class GameContainer: public Engine::Listener, public Engine::Producer {
public:
  GameContainer( Engine::Core *setCore );
  virtual ~GameContainer();
  static void OnKeyEvent( Engine::Listener* listener, Engine::Producer *producer, int eventId, void *data );
  static void OnMobileKeyPressed( Engine::Listener* listener, Engine::Producer *producer, int eventId, void *data );
  static void OnAppClose( Engine::Listener* listener, Engine::Producer *producer, int eventId, void *data );
  static void OnGraphicsInit( Engine::Listener* listener, Engine::Producer *producer, int eventId, void *data );
  static void OnAfterRender( Engine::Listener* listener, Engine::Producer *producer, int eventId, void *data );
  static void OnInitRender( Engine::Listener* listener, Engine::Producer *producer, int eventId, void *data );
  static void BeforeRender( Engine::Listener* listener, Engine::Producer *producer, int eventId, void *data );
  void AppExit();
  void AppSuspend();
  void _TestFile();
  void _TestImageLoader();
  void _TestTexture();
  void _TestMesh();

  Object* CreateObject( const std::string &name, const std::string &materialName, const Vec3& position = Vec3Null, const Vec2& scale = Vec2One, const float rotation = 0.0f );
  Object* CreateSprite( const std::string &name, const std::string &materialName, const Vec2& size, const Vec3& position = Vec3Null, const Vec2& scale = Vec2One, const float rotation = 0.0f );
  Object* CreateBacterium( const std::string &name, const std::string &materialName, const Vec2& size, const Vec3& position = Vec3Null, const Vec2& scale = Vec2One, const float rotation = 0.0f, const int bonesCount = 3 );
  void DeleteObject( Object *obj );
  void DeleteObject( const std::string& objectName );
  Engine::Material* CreateMaterial( const std::string& name, Engine::ShaderProgram *shader );
  Engine::Material* CreateMaterial( const std::string& name, Engine::ShaderProgram *shader, const std::string textureFileName, Engine::TextureFilterType filter );
  Engine::Material* CreateMaterial( const std::string& name, Engine::ShaderProgram *shader, const std::string textureFileName, const Vec4& color, Engine::TextureFilterType filter );
  Engine::Material* GetMaterial( const std::string& name );
  Engine::ShaderProgram* CreateShader( const std::string& name, const std::string& vertexShaderFileName, const std::string& fragmentShaderFileName );
  Engine::ShaderProgram* GetShader( const std::string& name, bool supressWarning = false );
  Engine::Texture* CreateTexture( const std::string &name, size_t setWidth, size_t setHeight, unsigned char *data, bool setIsTransparent = false, bool setIsCompressed = false, size_t setDataLength = 0, Engine::ImageType setImageFormat = Engine::IMAGE_TYPE_UNKNOWN, Engine::TextureFilterType filter = Engine::TextureFilterType::TEXTURE_FILTER_TYPE_POINT );
  Engine::Texture* CreateTexture( const std::string &fileName, Engine::TextureFilterType filter = Engine::TextureFilterType::TEXTURE_FILTER_TYPE_POINT );
  Engine::Texture* GetTexture( const std::string &name, bool supressWarning = false );

  Engine::Core *core;
  Engine::Camera2d
    cameraMain,
    cameraGUI;
  Engine::Camera3d
    camera3D;

  static Engine::PerObjectShaderBuffer *objectsMatricesList;

private:
  GameContainer();
  GameContainer( GameContainer& );
  GameContainer& operator=( GameContainer& );

  //test
  //Engine::VertexBufferGL *buffer;
  typedef std::unordered_map< std::string, Engine::Material* > MaterialListType;
  typedef std::unordered_map< std::string, Engine::ShaderProgram* > ShaderListType;
  typedef std::unordered_map< std::string, Object* > ObjectListType;
  typedef std::unordered_map< std::string, Engine::Texture* > TextureListType;

  MaterialListType materialsList;
  ShaderListType shadersList;
  ObjectListType objectsList;
  TextureListType texturesList;
};

GameContainer *game = NULL;

class Object: public Engine::Mesh {
public:
  Object( const std::string &setName, Engine::Renderer *renderer, Engine::Material *material );
  virtual ~Object();
  inline std::string GetName() const { return this->name; }
  Object* SetPosition( const Vec3& setPosition );
  Object* SetScale( const Vec2& setScale );
  Object* SetSize( const Vec2& setSize );
  Object* SetRotation( const float setRotation );
  inline float GetRotation() const { return this->rotation; };
  virtual void BeforeRender();
  virtual void _Test() {}

private:
  Object();
  Object( Object& );
  Object& operator=( Object& );

  std::string name;
  Vec3 position;
  Vec2 scale;
  Vec2 size;
  float rotation;
  Mat4 matrix;
  bool matrixChanged;
  Engine::PerObjectShaderBuffer::IndexType objectMatrixIndex;
};


class ObjectBacteriumBone {
public:
  ObjectBacteriumBone();
  ObjectBacteriumBone( const ObjectBacteriumBone& bone );
  ObjectBacteriumBone& operator=( ObjectBacteriumBone& bone );

  Vec3
    basePosition,
    position;
  float tension;
  struct VerticeInfo {
    GLushort index;
    float morphPower;
    Vec3 srcPosition;

    VerticeInfo( const GLushort setIndex, const float setMorphPower, const Vec3 &position )
      :index( setIndex ), morphPower( setMorphPower ), srcPosition( position )
    { }
  };
  typedef std::deque< VerticeInfo > VerticesList;
  VerticesList vertices;
};


class ObjectBacterium: public Object, public Engine::Listener {
public:
  ObjectBacterium( const std::string &setName, Engine::Renderer *renderer, Engine::Material *material, const float radius, const int bonesCount );
  virtual ~ObjectBacterium();
  static void Update( Engine::Listener* listener, Engine::Producer *producer, int eventId, void *data );

  virtual void _Test();

private:
  ObjectBacterium();
  ObjectBacterium( ObjectBacterium& );
  ObjectBacterium& operator=( ObjectBacterium& );
  void RecalculateBones();

  typedef std::deque< ObjectBacteriumBone* > BonesList;

  BonesList bones;
  ObjectBacteriumBone::VerticesList defaultVerticesPosition;
  float rotationSpeed;
};

};  //namespace Game
