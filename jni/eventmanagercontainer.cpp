#include "eventmanagercontainer.h"
#include "listener.h"
#include "tools.h"

using namespace Engine;

EventManagerContainer::EventManagerContainer()
:eventManager( NULL ) {
}

EventManagerContainer::~EventManagerContainer() {
  SAFE_DELETE( this->eventManager );
}

void EventManagerContainer::Listen( Listener *listener, const int eventId, ListenerProc *listenerProc ) {
  listener->AddEventManager( this->GetEventManager() );
  this->GetEventManager()->AddListener( listener, eventId, listenerProc );
}//Listen

EventManager* EventManagerContainer::GetEventManager() {
  if( !this->eventManager ) {
    this->eventManager = new EventManager();
  }

  return this->eventManager;
}//GetEventManager
