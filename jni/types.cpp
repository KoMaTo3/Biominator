#include "types.h"

using namespace Engine;

Position::Position()
:x(), y() {
}

Position::Position( const int setX, const int setY )
:x( setX ), y( setY ) {
}

Position::~Position() {
}

Size::Size()
:width(), height() {
}

Size::Size( const int setWidth, const int setHeight )
:width( setWidth ), height( setHeight ) {
}

Size::~Size() {
}
