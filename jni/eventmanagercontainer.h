#pragma once

#include "eventmanager.h"

namespace Engine {

class EventManagerContainer {
public:
  EventManagerContainer();
  virtual ~EventManagerContainer();
  void Listen( Listener *listener, const int eventId, ListenerProc *listenerProc );

protected:
  EventManager* GetEventManager();
  inline bool EventManagerInitialized() const { return this->eventManager != 0; };

private:
  EventManagerContainer( EventManagerContainer& );
  EventManagerContainer& operator=( EventManagerContainer& );
  EventManager *eventManager;
};

};
