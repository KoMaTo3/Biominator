#include "types.h"

using namespace Engine;

Position::Position() {
}

Position::Position( const int setX, const int setY )
:x( setX ), y( setY ) {
}

Position::~Position() {
}

Size::Size() {
}

Size::Size( const int setWidth, const int setHeight )
:width( setWidth ), height( setHeight ) {
}

Size::~Size() {
}
