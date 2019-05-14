#pragma once

#include <cstdint>

#include <Shared/CRC32.h>

#define DEFINE_SERID(id)               \
	uint32_t Id() override {           \
		constexpr uint32_t _id = id;   \
		return _id;                    \
	}

namespace uf {

class Archive;

class ISerializable {
public:
	virtual uint32_t Id() = 0;
	virtual void Serialize(Archive &archive);

	virtual ~ISerializable() = default;
};

}
