#include "entrypoint.h"

using namespace Engine;

EntryPoint::EntryPoint() {
}

EntryPoint::~EntryPoint() {
}

void EntryPoint::Run( Core* core ) {
  this->Init( core );
  core->Run();
  delete core;
}
