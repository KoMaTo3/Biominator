#pragma once

#include "math/kvector.h"
#include "math/kmatrix.h"

namespace Engine {

class Camera3d {
public:
  Camera3d();
  virtual ~Camera3d();
  Camera3d* SetPosition( const Vec3& setPosition );
  Camera3d* SetScale( const Vec2& setScale );
  Camera3d* SetRotation( const float setRotation );
  inline const Vec3& GetPosition() const { return this->position; }
  virtual void Update();
  inline float* GetMatrixPointer() { return &this->matrix[ 0 ][ 0 ]; }

private:
  Camera3d( Camera3d& );
  Camera3d& operator=( Camera3d& );

  Vec3 position;
  bool matrixChanged;
  Mat4 matrix;
};

}
