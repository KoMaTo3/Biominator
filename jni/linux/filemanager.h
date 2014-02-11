#pragma once

#include "../filemanager.h"

namespace Engine {

class FileManagerLinux;
typedef FileManagerLinux FileManagerType;

class FileManagerLinux: public FileManager {
public:
  FileManagerLinux( const std::string& setRootDir );
  virtual ~FileManagerLinux();
  virtual bool FileExists( const std::string &fileName ) const;
  virtual bool GetFile( const std::string& fileName, Memory& fileContent, bool endZeroByte = false ) const;

private:
  FileManagerLinux();
  FileManagerLinux( FileManagerLinux& );
  FileManagerLinux& operator=( FileManagerLinux& );
};

};
