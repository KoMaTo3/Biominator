#pragma once

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
  Renderer *renderer;

private:
  Core( const Core& );
  Core& operator=( const Core& );
};

};
