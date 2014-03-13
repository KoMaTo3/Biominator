#pragma once

#include "math/kvector.h"
#include "math/kmatrix.h"

namespace Engine {

class Camera {
public:
  Camera();
  virtual ~Camera();
  Camera* SetPosition( const Vec3& setPosition );
  Camera* SetScale( const Vec2& setScale );
  Camera* SetRotation( const float setRotation );
  inline const Vec3& GetPosition() const { return this->position; }
  inline const Vec2& GetScale() const { return this->scale; }
  inline const float GetRotation() const { return this->rotation; }
  virtual void Update();
  inline float* GetMatrixPointer() { return &this->matrix[ 0 ][ 0 ]; }

private:
  Camera( Camera& );
  Camera& operator=( Camera& );

  Vec3 position;
  Vec2 scale;
  float rotation;
  bool matrixChanged;
  Mat4 matrix;
};

}
