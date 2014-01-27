#include "eventmanager.h"
#include "listener.h"
#include "tools.h"

using namespace Engine;

ListenerHandler::ListenerHandler( int setEventId, Listener* setListener, ListenerProc *setListenerProc )
:eventId( setEventId ), listener( setListener ), proc( setListenerProc ) {
}


EventManager::EventManager() {
}

EventManager::~EventManager() {
}

void EventManager::AddListener( Listener *listener, const int eventId, ListenerProc *listenerProc ) {
  if( !this->ListenerExists( listener, eventId ) ) {
    this->listenersList.push_back( ListenerHandler( eventId, listener, listenerProc ) );
  }
}//AddListener

bool EventManager::ListenerExists( const Listener *listener, const int eventId ) const {
  for( auto& item: this->listenersList ) {
    if( item.eventId == eventId && item.listener == listener ) {
      return true;
    }
  }
  return false;
}//ListenerExists

void EventManager::RemoveListener( const Listener *listener ) {
  auto
    iter = this->listenersList.begin(),
    iterEnd = this->listenersList.end();
  while( iter != iterEnd ) {
    if( iter->listener == listener ) {
      this->listenersList.erase( iter );
      break;
    }
    ++iter;
  }
}//RemoveListener

void EventManager::RemoveListener( const Listener *listener, const int eventId ) {
  auto
    iter = this->listenersList.begin(),
    iterEnd = this->listenersList.end();
  while( iter != iterEnd ) {
    if( iter->eventId == eventId && iter->listener == listener ) {
      this->listenersList.erase( iter );
      break;
    }
    ++iter;
  }
}//RemoveListener

void EventManager::TouchEvent( const int eventId, Producer* producer, void *data ) {
  ListenerVector listenersListProc;
  for( auto& listener: this->listenersList ) {
    if( listener.eventId == eventId ) {
      listenersListProc.push_back( listener );
    }
  }

  for( auto& listener: listenersListProc ) {
    listener.proc( listener.listener, producer, eventId, data );
  }
}//TouchEvent
