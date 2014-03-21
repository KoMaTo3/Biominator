#include "mesh.h"
#include "material.h"
#include "shaderprogram.h"
#include "tools.h"
#include "interngl.h"

using namespace Engine;

Mesh::Mesh( Renderer *setRenderer, Material *setMaterial )
:vertices( NULL ), material( setMaterial ), vertexBuffer( setRenderer ), objectMatrix( NULL ), customWorldMatrix( NULL ), customProjectionMatrix( NULL ) {
}


Mesh::~Mesh() {
}

void Mesh::Render( float *worldMatrix ) {
  if( !this->vertices || !this->vertices->GetSize() ) {
    return;
  }

  LOGI( "Mesh::Render begin" );
  LOGI( "apply material..." );
  if( !this->material ) {
    LOGE( "Mesh::Render => material is NULL" );
    return;
  }
  this->material->Apply();
  LOGI( "apply vbo..." );
  this->vertexBuffer.Bind();

  this->BeforeRender();

  LOGI( "apply attribs, vPosition[%d] vTex[%d]", this->material->shaderProgram->GetAttribLocation( "vPosition" ), this->material->shaderProgram->GetAttribLocation( "vTex" ) );
  glVertexAttribPointer( this->material->shaderProgram->GetAttribLocation( "vPosition" ), 3, GL_FLOAT, GL_FALSE, sizeof( Vertice ), ( void* ) ( this->vertices->GetBeginIndex() * sizeof( Vertice ) + FIELD_OFFSET( Vertice, pos ) ) );
  glEnableVertexAttribArray( this->material->shaderProgram->GetAttribLocation( "vPosition" ) );
  glVertexAttribPointer( this->material->shaderProgram->GetAttribLocation( "vTex" ), 2, GL_FLOAT, GL_FALSE, sizeof( Vertice ), ( void* ) ( this->vertices->GetBeginIndex() * sizeof( Vertice ) + FIELD_OFFSET( Vertice, tex ) ) );
  glEnableVertexAttribArray( this->material->shaderProgram->GetAttribLocation( "vTex" ) );

  if( this->customProjectionMatrix ) {
    glUniformMatrix4fv( this->material->shaderProgram->GetUniformLocation( "projMatrix" ), 1, false, this->customProjectionMatrix );
    LOGI( "projMatrix uniform = %d", this->material->shaderProgram->GetUniformLocation( "projMatrix" ) );
  }

  if( this->customWorldMatrix ) {
    glUniformMatrix4fv( this->material->shaderProgram->GetUniformLocation( "worldMatrix" ), 1, false, this->customWorldMatrix );
  } else if( worldMatrix ) {
    glUniformMatrix4fv( this->material->shaderProgram->GetUniformLocation( "worldMatrix" ), 1, false, worldMatrix );
  }

  if( this->objectMatrix ) {
    glUniformMatrix4fv( this->material->shaderProgram->GetUniformLocation( "objMatrix" ), 1, false, this->objectMatrix );
  }
  
  LOGI( "draw %d points...", this->vertices->GetSize() );
  glDrawArrays( GL_TRIANGLES, this->vertices->GetBeginIndex(), this->vertices->GetSize() );
  LOGI( "Mesh::Render end" );
}//Render

VertexBufferWriter* Mesh::ResizeVertexBuffer( unsigned short setSize ) {
  if( this->vertices ) {
    this->vertexBuffer.Delete( this->vertices );
  }
  this->vertices = this->vertexBuffer.New( setSize );

  return this->vertices;
}//ResizeVertexBuffer

void Mesh::BeforeRender() {
}
