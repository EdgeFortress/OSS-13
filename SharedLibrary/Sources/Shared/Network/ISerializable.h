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

#define DEFINE_SERIALIZABLE(TypeName, ParentName) \
	struct TypeName : public ParentName {         \
		DEFINE_SERID(#TypeName##_crc32)
#define DEFINE_SERIALIZABLE_END };


