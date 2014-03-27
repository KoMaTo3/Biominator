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

Size::Size( const uint32_t &newWidth, const uint32_t &newHeight )
:width( newWidth ), height( newHeight ) {
}
