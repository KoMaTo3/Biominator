#include "entrypoint.h"
#include "math/kmath.h"

using namespace Engine;

EntryPoint::EntryPoint() {
  Math::Init();
}

EntryPoint::~EntryPoint() {
}

void EntryPoint::Run( Core* core ) {
  this->Init( core );
  core->Run();
  this->Destroy();
}
