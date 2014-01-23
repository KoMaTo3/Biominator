#pragma once

void OnEngineRun();

namespace Engine {

class Renderer;

class Core {
public:
  Core();
  virtual ~Core();
  virtual void Update() = 0;
  virtual void Destroy() = 0;
  virtual void Run();

protected:
  bool isValid;
  bool isFocused;
  bool isVisible;
  Renderer *renderer;
  unsigned int screenWidth;
  unsigned int screenHeight;

private:
  Core( const Core& );
  Core& operator=( const Core& );
};

};
