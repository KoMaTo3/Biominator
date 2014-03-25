#pragma once

#include "platform/opengl.h"

#include <string>
#include <vector>

namespace Engine {

class ShaderProgram
{
public:
  ShaderProgram();
  virtual ~ShaderProgram();
  bool AttachVertexShader( const char *content, const size_t length );
  bool AttachFragmentShader( const char *content, const size_t length );
  GLint GetUniformLocation( const std::string& uniformName ) const;
  GLint GetAttribLocation( const std::string& attribName ) const;
  void UseProgram();

private:
  typedef std::vector< GLuint > ShaderProgramList;
  GLuint program;
  ShaderProgramList shadersList;

  ShaderProgram( ShaderProgram& );
  ShaderProgram& operator=( ShaderProgram& );
  bool _CheckShaderError( const std::string& text, GLuint shader );
  bool _AttachShader( const GLchar *content, const GLint length, GLenum shaderType );
};

};
