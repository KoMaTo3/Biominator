#include "renderer.h"
#include "tools.h"

using namespace Engine;

Renderer::Renderer( const unsigned int setScreenWidth, const unsigned int setScreenHeight )
  :isValid( false ), screenWidth( setScreenWidth ), screenHeight( setScreenHeight ) {
}

Renderer::~Renderer() {
}
