#pragma once

#include <cstdint>
#include <memory>

#include <Shared/CRC32.h>
#include <Shared/IFaces/ICopyable.h>

#define _DEFINE_SERID(id)                  \
	uint32_t Id() override {               \
		constexpr uint32_t _id = id;       \
		return _id;                        \
	}                                      \
	constexpr static uint32_t StaticId() { \
		return id;                         \
	}

namespace uf {

class Archive;

class ISerializable : public ICopyable {
public:
	virtual uint32_t Id() = 0;
	virtual void Serialize(Archive &archive);

	virtual ~ISerializable() = default;
};

std::unique_ptr<ISerializable> CreateSerializableById(uint32_t id);

}

// Start of serializable struct definition
//	TypeName - Name of serializable struct
//	ParentName - Name of inherited serializable (should be at least uf::ISerializable)
#define DEFINE_SERIALIZABLE(TypeName, ParentName) \
	struct TypeName : public ParentName {         \
		_DEFINE_SERID(#TypeName##_crc32)

// End of serializable struct definition
#define DEFINE_SERIALIZABLE_END };

// Define serializable struct without fields
#define DEFINE_PURE_SERIALIZABLE(TypeName, ParentName) \
	DEFINE_SERIALIZABLE(TypeName, ParentName)          \
	DEFINE_SERIALIZABLE_END
