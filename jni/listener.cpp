#include "tools.h"
#include "listener.h"
#include "eventmanager.h"

using namespace Engine;

Listener::Listener() {
  LOGI( "Listener, this[%p]", this );
}


Listener::~Listener() {
  LOGI( "~Listener, this[%p]", this );
  while( !this->_eventManagersList.empty() ) {
    auto manager = this->_eventManagersList.begin();
    LOGI( "~Listener, this[%p] manager[%p]", this, *manager );
    ( *manager )->RemoveListener( this );
    LOGI( "~Listener, this[%p] manager[...] done", this );
  }
  /*
  while( !this->_eventManagersList.empty() ) {
    auto manager = this->_eventManagersList.begin();
    LOGI( "~Listener => manager[%p]", *manager );
    ( *manager )->RemoveListener( this );
  }
  this->_eventManagersList.clear();
  */
  LOGI( "~Listener done, this[%p]", this );
}


void Listener::AddEventManager( EventManager* setManager ) {
  for( auto& manager: this->_eventManagersList ) {
    if( manager == setManager ) {
      return;
    }
  }
  this->_eventManagersList.push_back( setManager );
}//AddEventManager


void Listener::RemoveEventManager( EventManager* manager ) {
  LOGI( "Listener::RemoveEventManager => this[%p] manager[%p]", this, manager );
  for( auto &iter: this->_eventManagersList ) {
    if( iter == manager ) {
      LOGI( "+" );
    }
  }
  auto
    iter = this->_eventManagersList.begin(),
    iterEnd = this->_eventManagersList.end();
  while( iter != iterEnd ) {
    if( *iter == manager ) {
      this->_eventManagersList.erase( iter );
      LOGI( "Listener::RemoveEventManager => this[%p] manager[%p] all[%d] done", this, manager, this->_eventManagersList.size() );
      return;
    }
    ++iter;
  }
  LOGI( "Listener::RemoveEventManager => manager %p not found", manager );
}//AddEventManager
