#pragma once

#include <cstdint>
#include <queue>

class IHasRepeatableID {
public:
	uint32_t ID() const;

protected:
	IHasRepeatableID();
	virtual ~IHasRepeatableID();

private:
	const uint32_t id;
};
