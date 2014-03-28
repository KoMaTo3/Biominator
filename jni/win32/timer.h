#pragma once

#include "../timer.h"

namespace Engine {

class TimerWin32: public ITimer {
public:
  TimerWin32();
  virtual ~TimerWin32();
  virtual double Update();

private:
  TimerWin32( TimerWin32& );
  TimerWin32& operator=( TimerWin32& );

  void UpdateFreq();
  void UpdateCounter();

  __int64 frequency;
  __int64 tick;
  double inverceFrequency;
};

};
