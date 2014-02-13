#include "mesh.h"
#include "material.h"
#include "shaderprogram.h"

using namespace Engine;

Mesh::Mesh( Renderer *setRenderer, Material *setMaterial )
:vertexBuffer( setRenderer ), material( setMaterial ), vertices( NULL ) {
}


Mesh::~Mesh() {
}

void Mesh::Render() {
  if( !this->vertices || !this->vertices->GetSize() ) {
    return;
  }

  LOGI( "Mesh::Render begin" );
  LOGI( "apply material..." );
  this->material->Apply();
  LOGI( "apply vbo..." );
  this->vertexBuffer.Bind();

  LOGI( "apply attribs, vPosition[%d] vTex[%d]", this->material->shaderProgram->GetAttribLocation( "vPosition" ), this->material->shaderProgram->GetAttribLocation( "vTex" ) );
  glVertexAttribPointer( this->material->shaderProgram->GetAttribLocation( "vPosition" ), 3, GL_FLOAT, GL_FALSE, sizeof( Vertice ), ( void* ) ( this->vertices->GetBeginIndex() * sizeof( Vertice ) ) );
  glEnableVertexAttribArray( this->material->shaderProgram->GetAttribLocation( "vPosition" ) );
  glVertexAttribPointer( this->material->shaderProgram->GetAttribLocation( "vTex" ), 2, GL_FLOAT, GL_FALSE, sizeof( Vertice ), ( void* ) ( this->vertices->GetBeginIndex() * sizeof( Vertice ) + sizeof( Vec3 ) ) );
  glEnableVertexAttribArray( this->material->shaderProgram->GetAttribLocation( "vTex" ) );
  
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
