#include "listener.h"
#include "eventmanager.h"

using namespace Engine;

Listener::Listener() {
}

Listener::~Listener() {
  for( auto& manager: this->_eventManagersList ) {
    manager->RemoveListener( this );
  }
  this->_eventManagersList.clear();
}

void Listener::AddEventManager( EventManager* setManager ) {
  for( auto& manager: this->_eventManagersList ) {
    if( manager == setManager ) {
      return;
    }
  }
  this->_eventManagersList.push_back( setManager );
}//AddEventManager
