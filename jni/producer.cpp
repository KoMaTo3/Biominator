#include "producer.h"
#include "eventmanager.h"
#include "tools.h"

using namespace Engine;

Producer::Producer()
:EventManagerContainer() {
  LOGI( "Producer %p", this );
}

Producer::~Producer() {
  LOGI( "~Producer %p", this );
  if( this->EventManagerInitialized() ) {
    this->GetEventManager()->RemoveProducerFromAllListeners();
  }
  /*
  if( this->EventManagerInitialized() ) {
    this->GetEventManager()->RemoveProducerFromAllListeners();
  }
  */
  LOGI( "~Producer %p done", this );
}

void Producer::TouchEvent( const int eventId, void *data ) {
  this->GetEventManager()->TouchEvent( eventId, this, data );
}//TouchEvent

void Producer::TouchEvent( Event* event ) {
  this->GetEventManager()->TouchEvent( event->GetID(), this, event );
}//TouchEvent
