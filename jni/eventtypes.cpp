#include "eventtypes.h"

using namespace Engine;

EventKey::EventKey()
:Event( EVENT_TYPE_KEY_PRESSED ) {
}

EventKey::EventKey( const int setEventId )
:Event( setEventId ) {
}

EventMouseMove::EventMouseMove()
:Event( EVENT_TYPE_MOUSE_MOVE ) {
}
