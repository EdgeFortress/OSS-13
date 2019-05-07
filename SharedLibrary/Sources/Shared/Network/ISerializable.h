#pragma once

#include <cstdint>

#include "Archive.h"

namespace uf {

class ISerializable {
public:
	virtual void Serialize(Archive &archive) = 0;

	virtual ~ISerializable() = default;
};

}
