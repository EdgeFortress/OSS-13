#include "IHasRepeatableID.h"

static std::queue<uint32_t> freeIDs;

uint32_t getNextID() {
	if (!freeIDs.empty()) {
		auto result = freeIDs.front();
		freeIDs.pop();
		return result;
	} else {
		static uint32_t nextID = 1;
		return nextID++;
	}
}

IHasRepeatableID::IHasRepeatableID() :
	id(getNextID())
{ }

IHasRepeatableID::~IHasRepeatableID() {
	freeIDs.push(id);
}

uint32_t IHasRepeatableID::ID() const {
	return id;
}
