#include "shaderprogram.h"
#include "tools.h"
#include "interngl.h"

using namespace Engine;

ShaderProgram::ShaderProgram()
: program( 0 ) {
  this->program = glCreateProgram();
}


bool ShaderProgram::_AttachShader( const GLchar *content, const GLint length, GLenum shaderType ) {
  if( !content ) {
    LOGE( "ShaderProgram::_AttachShader => content is NULL, shaderType[x%X]", ( int ) shaderType );
    return false;
  }

  GLuint shader = glCreateShader( shaderType );
  glShaderSource( shader, 1, &content, &length );
  glCompileShader( shader );
  glAttachShader( this->program, shader );
  glLinkProgram( this->program );
  this->_CheckShaderError( "ShaderProgram::_AttachShader", shader );
  this->shadersList.push_back( shader );

  return true;
}//_AttachShader


bool ShaderProgram::AttachVertexShader( const char *content, const size_t length ) {
  return this->_AttachShader( content, length, GL_VERTEX_SHADER );
}//AttachVertexShader

bool ShaderProgram::AttachFragmentShader( const char *content, const size_t length ) {
  return this->_AttachShader( content, length, GL_FRAGMENT_SHADER );
}//AttachFragmentShader


bool ShaderProgram::_CheckShaderError( const std::string& text, GLuint shader ) {
  bool result = false;
  if( !glIsShader( shader ) ) {
    LOGE( "[ERROR] %d is not a shader (%s)\n", shader, text.c_str() );
    return true;
  }
  int maxLength, logLength = 0;
  glGetShaderiv( shader, GL_INFO_LOG_LENGTH, &maxLength );
  char *log = new char[ maxLength + 1 ];
  log[ maxLength ] = 0;
  glGetShaderInfoLog( shader, maxLength, &logLength, log );
  if( log[ 0 ] ) {
    LOGE( "[ERROR] LightMgr::_CheckShaderError => %s: %s\n", text.c_str(), log );
    result = true;
  }
  delete [] log;
  return result;
}//_CheckShaderError


ShaderProgram::~ShaderProgram() {
  for( auto &shader: this->shadersList ) {
    glDetachShader( this->program, shader );
    glDeleteShader( shader );
  }
  glDeleteProgram( this->program );
}


GLint ShaderProgram::GetUniformLocation( const std::string& uniformName ) const {
  LOGI( "ShaderProgram::GetUniformLocation => program[%d] name['%s']", this->program, uniformName.c_str() );
  return glGetUniformLocation( this->program, uniformName.c_str() );
}//GetUniformLocation

GLint ShaderProgram::GetAttribLocation( const std::string& attribName ) const {
  return glGetAttribLocation( this->program, attribName.c_str() );
}//GetAttribLocation

void ShaderProgram::UseProgram() {
  glUseProgram( this->program );
}//UseProgram
