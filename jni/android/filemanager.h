#pragma once

#include "../filemanager.h"
#include <android_native_app_glue.h>

namespace Engine {

class FileManagerAndroid;
typedef FileManagerAndroid FileManagerType;

class FileManagerAndroid: public FileManager {
public:
  FileManagerAndroid( AAssetManager *setAssetManager );
  virtual ~FileManagerAndroid();
  virtual bool FileExists( const std::string &fileName ) const;
  virtual bool GetFile( const std::string& fileName, Memory& fileContent, bool endZeroByte = false ) const;

private:
  FileManagerAndroid();
  FileManagerAndroid( FileManagerAndroid& );
  FileManagerAndroid& operator=( FileManagerAndroid& );

  AAssetManager *assetManager;
};

};
