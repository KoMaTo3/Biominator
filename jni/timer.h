#pragma once

namespace Engine {

class ITimer {
public:
  virtual ~ITimer();
  virtual double Update() = 0;
  inline double GetDelta() { return this->elapsed; }
  inline float GetDeltaF() { return this->fElapsed; }
  inline double GetTime() { return this->currentTime; }

protected:
  double elapsed;
  float fElapsed;
  double currentTime;
};

};
