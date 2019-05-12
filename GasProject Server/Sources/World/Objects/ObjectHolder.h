#pragma once

#include <vector>

#include <Server.hpp>
#include <Shared/Types.hpp>

#include "Object.hpp"

class ObjectHolder {
public:
	virtual ~ObjectHolder() = default;

	template<typename T, typename... TArgs>
	T *CreateObject(Tile *tile = nullptr, TArgs&&... Args);

	template<typename T, typename... TArgs>
	T *CreateObject(apos tile, TArgs&&... Args);

private:
	uint32_t addObject(Object *);
	void placeTo(Object *, Tile *);
	Tile *getTile(apos);

protected: // TODO: make it private!
	std::vector<uptr<Object>> objects;
	std::vector<uint> free_ids;
};

namespace detail {

template<typename T>
class Factory : public T {
friend ObjectHolder;

protected:
	template<typename... TArgs>
	Factory(TArgs&&... Args) :
		T(std::forward<TArgs>(Args)...)
	{ }

	virtual ~Factory() { }
	T *GetObject() { return this; }
};

}

template<typename T, typename... TArgs>
T *ObjectHolder::CreateObject(Tile *tile, TArgs&&... Args) {
	auto *factory = new detail::Factory<T>(std::forward<TArgs>(Args)...);
	Object *obj = factory->GetObject(); // Object * is important! It's check for correct type.

	obj->id = addObject(obj);
	placeTo(obj, tile);

	obj->AfterCreation();
	return static_cast<T *>(obj);
}

template<typename T, typename... TArgs>
T *ObjectHolder::CreateObject(apos coords, TArgs&&... Args) {
	return CreateObject<T>(getTile(coords), std::forward<TArgs>(Args)...);
}
