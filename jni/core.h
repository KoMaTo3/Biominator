#pragma once

#include "producer.h"
#include "eventmanagercontainer.h"
#include "eventtypes.h"
#include <string>

void OnEngineRun();

namespace Engine {

class Renderer;

class Core: public Producer {
public:
  Core();
  virtual ~Core();
  virtual void Update() = 0;
  virtual void Destroy() = 0;
  virtual void Run();
  virtual void Stop() = 0;
  const std::string GetPlatformName() const;
  const PlatformType GetPlatformType() const;

  Renderer *renderer;

protected:
  bool isValid;
  bool isFocused;
  bool isVisible;
  bool animating;
  unsigned int screenWidth;
  unsigned int screenHeight;

private:
  Core( const Core& );
  Core& operator=( const Core& );
};

};
