#include "ObjectHolder.h"

#include <IGame.h>
#include <IScriptEngine.h>
#include <World/World.hpp>
#include <World/Map.hpp>
#include <World/Tile.hpp>
#include <World/Objects/Object.hpp>

Object *ObjectHolder::CreateScriptObject(const std::string &typeKey, Tile *tile) {
	auto obj = GGame->GetScriptEngine()->CreateObject(typeKey);
	if (!obj)
		return {};

	placeTo(obj, tile);

	return obj;
}

Object *ObjectHolder::CreateScriptObject(const std::string &typeKey, apos coords) {
	return CreateScriptObject(typeKey, getTile(coords));
}

void ObjectHolder::AddObject(std::shared_ptr<Object> obj) {
	if (free_ids.empty()) {
		objects.push_back(obj);
		obj->id = uint32_t(objects.size());
	} else {
		auto id = free_ids.back();
		free_ids.pop_back();
		objects[id - 1] = obj;
		obj->id = id;
	}
}

void ObjectHolder::placeTo(Object *obj, Tile *tile) {
	if (tile)
		tile->PlaceTo(obj);
}

Tile *ObjectHolder::getTile(apos coords) {
	return GGame->GetWorld()->GetMap()->GetTile(coords);
}
