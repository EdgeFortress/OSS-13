#include "ObjectHolder.h"

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