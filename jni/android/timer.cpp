#include "timer.h"

using namespace Engine;

TimerAndroid::TimerAndroid()
:ITimer() {
  this->UpdateFreq();
}


TimerAndroid::~TimerAndroid() {
}


void TimerAndroid::UpdateFreq() {
  this->frequency = 1.0 / double( CLOCKS_PER_SEC );
  this->clockCurrent = clock();
}//UpdateFreq


double TimerAndroid::Update() {
  this->clockLast = this->clockCurrent;
  this->clockCurrent = clock();
  this->elapsed = double( this->clockCurrent - this->clockLast ) * this->frequency;
  this->currentTime += this->elapsed;
  this->fElapsed = float( this->elapsed );

  return this->elapsed;
}//Update
