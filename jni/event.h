#pragma once

namespace Engine {

class Event {
public:
  Event( const int setId );
  virtual ~Event();
  inline int GetID() const { return this->id; }

private:
  Event();
  Event( Event& );
  Event& operator=( Event& );

  int id;
};

};
