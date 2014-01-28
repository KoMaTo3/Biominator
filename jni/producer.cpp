#include "producer.h"
#include "eventmanager.h"

using namespace Engine;

Producer::Producer()
:EventManagerContainer() {
}

Producer::~Producer() {
}

void Producer::TouchEvent( const int eventId, void *data ) {
  this->eventManager.TouchEvent( eventId, this, data );
}//TouchEvent

void Producer::TouchEvent( Event* event ) {
  this->eventManager.TouchEvent( event->GetID(), this, event );
}//TouchEvent
