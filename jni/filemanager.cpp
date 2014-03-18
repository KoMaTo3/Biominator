#include "filemanager.h"
#include "string.h"

using namespace Engine;

FileManager::FileManager() {
}

FileManager::FileManager( const std::string &setRootDir )
: rootDir( setRootDir ) {
}

void FileManager::InitList( const Memory &fileData ) {
  ParseFileStruct parser( &fileData );
  std::string str;
  while( this->_GetNextString( parser, str ) ) {
    if( str.empty() || ( str.length() > 1 && str[ 0 ] == '/' && str[ 1 ] == '/' ) ) {
      continue;
    }
    this->ParseAndAddStringToFileList( str );
  }
}//InitList

FileManager::~FileManager() {
}

bool FileManager::FileHasOtherName( const std::string& srcFileName, std::string &outFileName ) const {
  auto iter = this->filesReplaceList.find( srcFileName );
  if( iter != this->filesReplaceList.end() ) {
    outFileName = iter->second;
    return true;
  }

  return false;
}//FileHasOtherName

bool FileManager::_GetNextString( ParseFileStruct &fileParser, std::string &outString ) {
  if( !fileParser.IsCorrect() ) {
    return false;
  }

  outString = "";
  size_t
    posStart = ( fileParser.position ? fileParser.position : 0 ),
    pos = fileParser.position;
  uint8_t *data = fileParser.fileData->GetData() + posStart;
  while( pos < fileParser.fileData->GetLength() && ( *data == '\n' || *data == '\r' ) ) {
    ++pos;
    ++data;
    ++posStart;
  }
  for(; pos < fileParser.fileData->GetLength(); ++pos, ++data ) {
    if( *data == '\n' || *data == '\r' ) {
      break;
    }
  }
  fileParser.position = pos;
  if( posStart == pos ) {
    return false;
  }

  Memory tmp( pos - posStart + 1 );
  memcpy( tmp.GetData(), fileParser.fileData->GetData() + posStart, pos - posStart );
  tmp[ tmp.GetLength() - 1 ] = 0;
  outString = ( char* ) tmp.GetData();

  return true;
}//_GetNextString

void FileManager::ParseAndAddStringToFileList( const std::string &str ) {
  size_t pos = str.find( ":" );
  if( !pos ) {
    LOGW( "FileManager::ParseAndAddStringToFileList => bad string format '%s'", str.c_str() );
    return;
  }

  this->filesReplaceList.insert( std::make_pair( str.substr( 0, pos ), str.substr( pos + 1, str.length() - pos - 1 ) ) );
}//ParseAndAddStringToFileList

FileManager::ParseFileStruct::ParseFileStruct( const Memory *setFileData )
:position( 0 ), fileData( setFileData ) {
}
