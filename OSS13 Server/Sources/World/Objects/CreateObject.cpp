#include "CreateObject.h"

#include <IGame.h>
#include <World/World.hpp>
#include <World/Tile.hpp>

Object *CreateObject(const std::string &module, Tile *tile) {
	return GGame->GetWorld()->CreateScriptObject(module, tile);
}

Object *CreateObject(const std::string &module, apos coords) {
	return GGame->GetWorld()->CreateScriptObject(module, coords);
}

Object *CreateObject(const std::string &module, uf::vec2i pos) {
	return CreateObject(module, apos(pos, 0));
}
