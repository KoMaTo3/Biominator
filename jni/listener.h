#pragma once

#include <deque>

namespace Engine {

class EventManager;
typedef std::deque< EventManager* > EventManagerDeque;

class Listener {
public:
  Listener();
  virtual ~Listener();
  void AddEventManager( EventManager* setManager );
  void RemoveEventManager( EventManager* manager );

private:
  Listener( Listener& );
  Listener& operator=( Listener& );

  EventManagerDeque _eventManagersList;
};

};