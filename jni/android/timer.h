#pragma once

#include "../timer.h"
#include <time.h>

namespace Engine {

class TimerAndroid: public ITimer {
public:
  TimerAndroid();
  virtual ~TimerAndroid();
  virtual double Update();

private:
  TimerAndroid( TimerAndroid& );
  TimerAndroid& operator=( TimerAndroid& );

  void UpdateFreq();

  clock_t
    clockCurrent,
    clockLast;
  double frequency;
};

};
