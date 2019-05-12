#include "ObjectHolder.h"

#include <World/World.hpp>
#include <World/Map.hpp>
#include <World/Tile.hpp>

uint32_t ObjectHolder::addObject(Object *obj) {
	if (free_ids.empty()) {
		objects.push_back(uptr<Object>(obj));
		return uint32_t(objects.size());
	} else {
		auto id = free_ids.back();
		free_ids.pop_back();
		objects[id - 1] = uptr<Object>(obj);
		return id;
	}
}

void ObjectHolder::placeTo(Object *obj, Tile *tile) {
	if (tile)
		tile->PlaceTo(obj);
}

Tile *ObjectHolder::getTile(apos coords) {
	return CurThreadGame->GetWorld()->GetMap()->GetTile(coords);
}
