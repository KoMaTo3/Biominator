#pragma once


namespace Engine {

class Renderer {
public:
  Renderer( const unsigned int setScreenWidth, const unsigned int setScreenHeight );
  virtual ~Renderer();
  virtual void Render() = 0;
  virtual void InitViewport() = 0;
  inline bool IsValid() { return this->isValid; }

protected:
  bool isValid;
  unsigned int screenWidth;
  unsigned int screenHeight;

private:
  Renderer();
  Renderer( const Renderer& );
  Renderer& operator=( const Renderer& );
};

};
