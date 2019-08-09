#pragma once

#include <vector>

#include <Shared/Types.hpp>

#include "Object.hpp"

class ObjectHolder {
public:
	virtual ~ObjectHolder() = default;

	template<typename T, typename... TArgs>
	T *CreateObject(Tile *tile = nullptr, TArgs&&... Args);

	template<typename T, typename... TArgs>
	T *CreateObject(apos tile, TArgs&&... Args);

	Object *CreateScriptObject(const std::string &module, Tile *tile = nullptr);
	Object *CreateScriptObject(const std::string &module, apos coords);

	void AddObject(std::shared_ptr<Object> obj);

private:
	void placeTo(Object *, Tile *);
	Tile *getTile(apos);

protected: // TODO: make it private!
	std::vector<sptr<Object>> objects;
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
	T *GetObject() { return static_cast<T *>(this); }
};

}

template<typename T, typename... TArgs>
T *ObjectHolder::CreateObject(Tile *tile, TArgs&&... Args) {
	auto *factory = new detail::Factory<T>(std::forward<TArgs>(Args)...);
	auto *obj = static_cast<Object *>(factory->GetObject());

	AddObject(std::shared_ptr<Object>(obj));
	placeTo(obj, tile);

	return static_cast<T *>(obj);
}

template<typename T, typename... TArgs>
T *ObjectHolder::CreateObject(apos coords, TArgs&&... Args) {
	return CreateObject<T>(getTile(coords), std::forward<TArgs>(Args)...);
}
