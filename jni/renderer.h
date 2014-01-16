#pragma once


namespace Engine {

class Renderer {
public:
  Renderer();
  virtual ~Renderer();
  virtual void Render() = 0;
};

};
