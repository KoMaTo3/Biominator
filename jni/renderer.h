#pragma once

#include "producer.h"
#include "math/kvector.h"

namespace Engine {

class Renderer: public Producer {
public:
  Renderer( const unsigned int setScreenWidth, const unsigned int setScreenHeight );
  virtual ~Renderer();
  virtual void Render() = 0;
  virtual void InitViewport() = 0;
  inline bool IsValid() { return this->isValid; }
  inline const Vec2 GetScreenSize() const { return Vec2( ( float) this->screenWidth, ( float) this->screenHeight ); }
  inline const unsigned int GetScreenWidth() const { return this->screenWidth; }
  inline const unsigned int GetScreenHeight() const { return this->screenHeight; }

protected:
  bool isValid;
  unsigned int screenWidth;
  unsigned int screenHeight;

  virtual void InitExtensions() = 0;

private:
  Renderer();
  Renderer( const Renderer& );
  Renderer& operator=( const Renderer& );
};

};
