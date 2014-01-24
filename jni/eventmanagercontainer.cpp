#include "eventmanagercontainer.h"

using namespace Engine;

EventManagerContainer::EventManagerContainer() {
}

EventManagerContainer::~EventManagerContainer() {
}

void EventManagerContainer::Listen( Listener *listener, const int eventId, ListenerProc *listenerProc ) {
  this->eventManager.AddListener( listener, eventId, listenerProc );
}//Listen
