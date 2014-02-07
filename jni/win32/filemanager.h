#pragma once

#include "../filemanager.h"

namespace Engine {

class FileManagerWin32;
typedef FileManagerWin32 FileManagerType;

class FileManagerWin32: public FileManager {
public:
  FileManagerWin32( const std::string& setRootDir );
  virtual ~FileManagerWin32();
  virtual bool FileExists( const std::string &fileName ) const;
  virtual bool GetFile( const std::string& fileName, Memory& fileContent, bool endZeroByte = false ) const;

private:
  FileManagerWin32();
  FileManagerWin32( FileManagerWin32& );
  FileManagerWin32& operator=( FileManagerWin32& );
};

};
