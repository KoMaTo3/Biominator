#pragma once

#include "event.h"
#include <deque>
#include <vector>

namespace Engine {

class EventManager;

class Listener;
class Producer;
typedef void ( ListenerProc )( Listener* listener, Producer *producer, int eventId, void *data );
struct ListenerHandler {
  int eventId;
  Listener* listener;
  ListenerProc* proc;

  ListenerHandler( int setEventId, Listener* setListener, ListenerProc *setListenerProc );
};
typedef std::deque< ListenerHandler > ListenerDeque;
typedef std::vector< ListenerHandler > ListenerVector;

class EventManager {
public:
  EventManager();
  virtual ~EventManager();
  void AddListener( Listener *listener, const int eventId, ListenerProc *listenerProc );
  void RemoveListener( const Listener *listener );
  void RemoveListener( const Listener *listener, const int eventId );
  void RemoveAllListeners();
  bool ListenerExists( const Listener *listener, const int eventId ) const;
  void TouchEvent( const int eventId, Producer* producer, void *data );
  void RemoveProducerFromAllListeners();

private:
  EventManager( EventManager& );
  EventManager& operator=( EventManager& );

  ListenerDeque listenersList;
};

};
