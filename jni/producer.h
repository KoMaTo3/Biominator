#pragma once

#include "eventmanagercontainer.h"

namespace Engine {

class EventManager;

class Producer: public EventManagerContainer {
public:
  Producer();
  virtual ~Producer();
  void TouchEvent( const int eventId, void *data );
  void TouchEvent( Event* event );

private:
  Producer( Producer& );
  Producer& operator=( Producer& );
};

};
