#include "mesh.h"

using namespace Engine;

Mesh::Mesh( Renderer *setRenderer )
:material( NULL ), vertexBuffer( setRenderer ) {
}


Mesh::~Mesh() {
}
