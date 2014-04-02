#include "timer.h"
#include <windows.h>

using namespace Engine;

TimerWin32::TimerWin32()
:ITimer() {
  this->UpdateFreq();
  this->UpdateCounter();
  this->currentTime = 0.0;
}


TimerWin32::~TimerWin32() {
}


void TimerWin32::UpdateFreq() {
  static __int64 tmp;
  QueryPerformanceFrequency( ( PLARGE_INTEGER ) &tmp );
  this->frequency = tmp;
  this->inverceFrequency = 1.0 / double( tmp );
}//UpdateFreq


void TimerWin32::UpdateCounter() {
  static __int64 tmp;
  QueryPerformanceCounter( ( PLARGE_INTEGER ) &tmp );
  this->tick = tmp;
}//UpdateCounter


double TimerWin32::Update() {
  __int64 oldTick = this->tick;
  this->UpdateCounter();
  this->elapsed = double( this->tick - oldTick ) * this->inverceFrequency;
  this->currentTime += this->elapsed;
  this->fElapsed = float( elapsed );

  return this->elapsed;
}//Update
