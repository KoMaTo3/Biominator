#pragma once

#include "memory.h"
#include <string>

namespace Engine {

class FileManager {
public:
  FileManager();
  virtual ~FileManager();
  virtual bool FileExists( const std::string &fileName ) const = 0;
  virtual bool GetFile( const std::string& fileName, Memory& fileContent, bool endZeroByte = false ) const = 0;

protected:
  std::string rootDir;

private:
  FileManager( FileManager& );
  FileManager& operator=( FileManager& );
};

};
