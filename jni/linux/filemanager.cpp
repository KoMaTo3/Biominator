#include "filemanager.h"
#include "../tools.h"
#include <sys/stat.h>
#include <string>

using namespace Engine;

FileManagerLinux::FileManagerLinux( const std::string& setRootDir )
:FileManager( setRootDir ) {
  if( !this->rootDir.empty() ) {
    mkdir( this->rootDir.c_str(), S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH );
  }

  Memory fileContent;
  if( this->GetFile( ".list", fileContent, true ) ) {
    this->InitList( fileContent );
  }
}

FileManagerLinux::~FileManagerLinux() {
}

bool FileManagerLinux::FileExists( const std::string &fileName ) const {
  FILE *file;
  std::string name = this->rootDir + fileName;
  file = fopen( name.c_str(), "rb" );
  if( !file ) {
    LOGE( "File '%s' not found", name.c_str() );
    return false;
  }
  fclose( file );

  return true;
}//FileExists

bool FileManagerLinux::GetFile( const std::string& fileName, Memory& fileContent, bool endZeroByte ) const {
  std::string name, nameReplaced;
  if( this->FileHasOtherName( fileName, nameReplaced ) ) {
    name = this->rootDir + nameReplaced;
  } else {
    name = this->rootDir + fileName;
  }
  FILE *file;
  fileContent.Free();
  file = fopen( name.c_str(), "rb" );
  if( !file ) {
    LOGE( "File '%s' not found", name.c_str() );
    return false;
  }
  fseek( file, 0, SEEK_END );
  size_t fileSize = ftell( file );
  if( fileSize ) {
    fseek( file, 0, SEEK_SET );
    fileContent.Alloc( fileSize + ( endZeroByte ? 1 : 0 ) );
    fread( fileContent.GetData(), fileSize, 1, file );
    fileContent[ fileContent.GetLength() - 1 ] = 0;
  }
  fclose( file );

  return true;
}//GetFile
