#pragma once

#include <vector>

#include <Server.hpp>
#include <Shared/Types.hpp>

#include "Object.hpp"

class ObjectHolder {
public:
	template<typename T, typename... TArgs>
	T *CreateObject(Tile *tile = nullptr, TArgs&&... Args);

	template<typename T, typename... TArgs>
	T *CreateObject(uf::vec2i tile, TArgs&&... Args);

private:
	uint32_t addObject(Object *);

protected: // TODO: make it private!
	std::vector<uptr<Object>> objects;
	std::vector<uint> free_ids;
};

template<typename T, typename... TArgs>
T *ObjectHolder::CreateObject(Tile *tile, TArgs&&... Args) {
	Object *obj = new T(std::forward<TArgs>(Args)...); // Object * is important! It's check for correct type.

	obj->id = addObject(obj);

	if (tile)
		tile->PlaceTo(obj);

	obj->AfterCreation();
	return static_cast<T *>(obj);
}

template<typename T, typename... TArgs>
T *ObjectHolder::CreateObject(uf::vec2i coords, TArgs&&... Args) {
	Tile *tile = CurThreadGame->GetWorld()->GetMap()->GetTile(coords);
	return CreateObject<T>(tile, std::forward<TArgs>(Args)...);
}
