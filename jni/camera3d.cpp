#include "camera3d.h"
#include "tools.h"

using namespace Engine;

Camera3d::Camera3d()
:position( Vec3Null ), matrixChanged( true ) {
}

Camera3d::~Camera3d() {
}


Camera3d* Camera3d::SetPosition( const Vec3& setPosition ) {
  if( this->position.x != setPosition.x || this->position.y != setPosition.y || this->position.z != setPosition.z ) {
    this->matrixChanged = true;
  }
  this->position = setPosition;
  return this;
}//SetPosition

void Camera3d::Update() {
  if( this->matrixChanged ) {

    /*
    float
      left = -100.0f,
      right = 100.0f,
      top = 100.0f,
      bottom = -100.0f,
      far = 100.0f,
      near = 1.0f;

    this->matrix[0][0] = 2.0f*near/(right-left);
    this->matrix[0][1] = 0;
    this->matrix[0][2] = 0;
    this->matrix[0][3] = 0;

    this->matrix[1][0] = 0;
    this->matrix[1][1] = 2.0f*near/(top-bottom);
    this->matrix[1][2] = 0;
    this->matrix[1][3] = 0;

    this->matrix[2][0] = 0;
    this->matrix[2][1] = 0;
    this->matrix[2][2] = (far+near)/(far-near);
    this->matrix[2][3] = -2.0f * far* near /(far-near);

    this->matrix[3][0] = 0;
    this->matrix[3][1] = 0;
    this->matrix[3][2] = -1.0f;
    this->matrix[3][3] = 0;
    */

    Mat4 matScale;
    matScale.Identity();
    matScale[ 0 ][ 0 ] = 2.0f;

    Mat4 matTrans;
    matTrans.Identity();
    static float ttt = -2.5f;
    ttt -= 0.01f;
    matTrans[3][2] = ttt;

    static float rotation = 0.5f;
    rotation += 0.01f;
    float cosa = Math::Cos16( rotation );
    float sina = Math::Sin16( rotation );
    Mat4 matRotZ;
    matRotZ.Identity();
    matRotZ[ 0 ][ 0 ] = cosa;
    matRotZ[ 1 ][ 0 ] = -sina;
    matRotZ[ 0 ][ 1 ] = sina;
    matRotZ[ 1 ][ 1 ] = cosa;

    Mat4 matRotX;
    matRotX.Identity();
    matRotX[ 1 ][ 1 ] = cosa;
    matRotX[ 1 ][ 2 ] = -sina;
    matRotX[ 2 ][ 1 ] = sina;
    matRotX[ 2 ][ 2 ] = cosa;

    static float
      fov = 90.0f,
      zNear = 0.01f,
      zFar = 10000.0f,
      aspect = 1.0f;
    const float h = 1.0f/tan(fov*Math::PI/360.0f);
    float neg_depth = zNear-zFar;
 
    float *m = this->GetMatrixPointer();
    //fov += 0.1f;

    m[0] = h / aspect;
    m[1] = 0;
    m[2] = 0;
    m[3] = 0;
 
    m[4] = 0;
    m[5] = h;
    m[6] = 0;
    m[7] = 0;
 
    m[8] = 0;
    m[9] = 0;
    m[10] = (zFar + zNear)/neg_depth;
    m[11] = -1;
 
    m[12] = 0;
    m[13] = 0;
    m[14] = 2.0f*(zNear*zFar)/neg_depth;
    m[15] = 0;

    //this->matrix = matScale * matTrans * this->matrix;
    this->matrix = matRotX * matScale * matTrans * this->matrix;

    //this->matrix = this->matrix.Transpose();
    //this->matrix = matScale * this->matrix;
    //this->matrix = this->matrix * matScale * matTrans;
    LOGI("3d test");

    LOGI( "3d:\n[%3.3f; %3.3f; %3.3f; %3.3f]\n[%3.3f; %3.3f; %3.3f; %3.3f]\n[%3.3f; %3.3f; %3.3f; %3.3f]\n[%3.3f; %3.3f; %3.3f; %3.3f]",
      this->matrix[0][0], this->matrix[0][1], this->matrix[0][2], this->matrix[0][3],
      this->matrix[1][0], this->matrix[1][1], this->matrix[1][2], this->matrix[1][3],
      this->matrix[2][0], this->matrix[2][1], this->matrix[2][2], this->matrix[2][3],
      this->matrix[3][0], this->matrix[3][1], this->matrix[3][2], this->matrix[3][3]
      );
 
    /*
    Mat4 matScale, matTranslate, matRotation;
    matScale.Identity();
    matTranslate.Identity();
    matRotation.Identity();

    matTranslate[ 0 ][ 3 ] = -this->position.x;
    matTranslate[ 1 ][ 3 ] = -this->position.y;

    float cosa = Math::Cos16( this->rotation );
    float sina = Math::Sin16( this->rotation );
    matRotation[ 0 ][ 0 ] = cosa;
    matRotation[ 1 ][ 0 ] = -sina;
    matRotation[ 0 ][ 1 ] = sina;
    matRotation[ 1 ][ 1 ] = cosa;

    matScale[ 0 ][ 0 ] = this->scale.x;
    matScale[ 1 ][ 1 ] = this->scale.y;

    this->matrix = matScale * matRotation * matTranslate;
    */

    //this->matrixChanged = false;
  }
}//Update
