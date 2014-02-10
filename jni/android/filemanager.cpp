#include "filemanager.h"
#include "../tools.h"

using namespace Engine;

FileManagerAndroid::FileManagerAndroid( AAssetManager *setAssetManager )
:FileManager(), assetManager( setAssetManager ) {
  LOGI( "+FileManagerAndroid" );
}

FileManagerAndroid::~FileManagerAndroid() {
  LOGI( "~FileManagerAndroid" );
}

bool FileManagerAndroid::FileExists( const std::string &fileName ) const {
  LOGI( "FileManagerAndroid::FileExists..." );
  AAsset* file = AAssetManager_open( this->assetManager, fileName.c_str(), AASSET_MODE_BUFFER );
  if( !file ) {
    return false;
  }
  AAsset_close( file );
  return true;
}//FileExists

bool FileManagerAndroid::GetFile( const std::string& fileName, Memory& fileContent, bool endZeroByte ) const {
  fileContent.Free();
  AAsset* file = AAssetManager_open( this->assetManager, fileName.c_str(), AASSET_MODE_BUFFER );
  if( !file ) {
    LOGE( "FileManagerAndroid::GetFile => file '%s' not found", fileName.c_str() );
    return false;
  }
  off_t fileLength = AAsset_getLength( file );
  fileContent.Alloc( fileLength + ( endZeroByte ? 1 : 0 ) );
  AAsset_read( file, fileContent.GetData(), fileLength );
  AAsset_close( file );
  if( endZeroByte ) {
    fileContent[ fileContent.GetLength() - 1 ] = 0;
  }

  return true;
}//GetFile
