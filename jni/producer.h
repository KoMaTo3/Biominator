#pragma once

#include "eventmanagercontainer.h"

namespace Engine {

class EventManager;

class Producer: public EventManagerContainer {
public:
  Producer();
  virtual ~Producer();
  void TouchEvent( const int eventId );

private:
  Producer( Producer& );
  Producer& operator=( Producer& );
};

};