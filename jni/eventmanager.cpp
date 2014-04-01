#include "eventmanager.h"
#include "listener.h"
#include "tools.h"

using namespace Engine;

ListenerHandler::ListenerHandler( int setEventId, Listener* setListener, ListenerProc *setListenerProc )
:eventId( setEventId ), listener( setListener ), proc( setListenerProc ) {
}


EventManager::EventManager() {
  LOGI( "EventManager %p", this );
}

EventManager::~EventManager() {
  LOGI( "~EventManager %p", this );
  this->RemoveProducerFromAllListeners();
  this->RemoveAllListeners();
  LOGI( "~EventManager %p done", this );
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
  LOGI( "EventManager::RemoveListener => this[%p] listener[%p] all[%d]", this, listener, this->listenersList.size() );
  auto
    iter = this->listenersList.begin(),
    iterEnd = this->listenersList.end();
  std::deque< Listener* > removedListenersList;
  while( iter != iterEnd ) {
    LOGI( ". iter listener %p", iter->listener );
    if( iter->listener == listener ) {
      iter->listener->RemoveEventManager( this );
      bool finded;
      LOGI( ". erase" );
      do {
        finded = false;
        iterEnd = this->listenersList.end();
        for( iter = this->listenersList.begin(); iter != iterEnd; ++iter ) {
          if( iter->listener == listener ) {
            finded = true;
            this->listenersList.erase( iter );
            break;
          }
        }
      } while( finded );
      LOGI( ". erase done" );
      /*
      this->listenersList.erase( iter );
      iterEnd = this->listenersList.end();
      for( iter = this->listenersList.begin(); iter != iterEnd; ) {
        if( iter->listener == listener ) {
          auto iterNext = iter;
          ++iterNext;
          this->listenersList.erase( iter );
          iterEnd = this->listenersList.end();
          iter = iterNext;
        } else {
          ++iter;
        }
      }
      */
      break;
    } else {
      ++iter;
    }
  }
  LOGI( "EventManager::RemoveListener => this[%p] listener[%p] all[%d] done", this, listener, this->listenersList.size() );
}//RemoveListener

void EventManager::RemoveListener( const Listener *listener, const int eventId ) {
  auto
    iter = this->listenersList.begin(),
    iterEnd = this->listenersList.end();
  while( iter != iterEnd ) {
    if( iter->eventId == eventId && iter->listener == listener ) {
      //iter->listener->RemoveEventManager( this );
      this->listenersList.erase( iter );
      break;
    }
    ++iter;
  }
}//RemoveListener

void EventManager::RemoveAllListeners() {
  while( !this->listenersList.empty() ) {
    auto listener = this->listenersList.begin();
    LOGI( "EventManager::RemoveAllListeners %p", listener->listener );
    listener->listener->RemoveEventManager( this );
    this->listenersList.pop_front();
  }
}//RemoveAllListeners

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

void EventManager::RemoveProducerFromAllListeners() {
  LOGI( "EventManager::RemoveProducerFromAllListeners, manager[%p]", this );
  for( auto &listener: this->listenersList ) {
    //LOGI( "EventManager::RemoveProducerFromAllListeners, manager[%p] listener[%p]", this, listener.listener );
    listener.listener->RemoveEventManager( this );
  }
  /*
  for( auto &listener: this->listenersList ) {
    LOGI( "EventManager::RemoveProducerFromAllListeners, manager[%p] listener[%p]", this, listener.listener );
    listener.listener->RemoveEventManager( this );
  }
  this->listenersList.clear();
  */
  LOGI( "EventManager::RemoveProducerFromAllListeners done, manager[%p]", this );
}//RemoveProducerFromAllListeners
