#pragma once

#include "memory.h"
#include <string>
#include <unordered_map>

namespace Engine {

class FileManager {
public:
  FileManager();
  FileManager( const std::string &setRootDir );
  virtual ~FileManager();
  virtual bool FileExists( const std::string &fileName ) const = 0;
  virtual bool GetFile( const std::string& fileName, Memory& fileContent, bool endZeroByte = false ) const = 0;

protected:
  std::string rootDir;

  void InitList( const Memory &fileData );
  bool FileHasOtherName( const std::string& srcFileName, std::string &outFileName ) const;

private:
  class ParseFileStruct;
  FileManager( FileManager& );
  FileManager& operator=( FileManager& );
  bool _GetNextString( ParseFileStruct &fileParser, std::string &outString );
  void ParseAndAddStringToFileList( const std::string &str );

  std::unordered_map< std::string, std::string > filesReplaceList; //в соответствии с /data/.list список замещаемых имён файлов


  class ParseFileStruct {
  public:
    ParseFileStruct( const Memory *setFileData );
    size_t position;
    const Memory *fileData;
    inline bool IsCorrect() const { return this->fileData && this->fileData->GetLength() && this->position < this->fileData->GetLength(); }

  private:
    ParseFileStruct();
    ParseFileStruct( ParseFileStruct& );
    ParseFileStruct& operator=( ParseFileStruct& );
  };
};

};
