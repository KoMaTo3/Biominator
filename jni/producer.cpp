#include "producer.h"
#include "eventmanager.h"

using namespace Engine;

Producer::Producer()
:EventManagerContainer() {
}

Producer::~Producer() {
}

void Producer::TouchEvent( const int eventId ) {
  this->eventManager.TouchEvent( eventId, this );
}//TouchEvent
