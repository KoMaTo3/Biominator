#include "eventmanagercontainer.h"
#include "listener.h"

using namespace Engine;

EventManagerContainer::EventManagerContainer() {
}

EventManagerContainer::~EventManagerContainer() {
}

void EventManagerContainer::Listen( Listener *listener, const int eventId, ListenerProc *listenerProc ) {
  listener->AddEventManager( &this->eventManager );
  this->eventManager.AddListener( listener, eventId, listenerProc );
}//Listen
