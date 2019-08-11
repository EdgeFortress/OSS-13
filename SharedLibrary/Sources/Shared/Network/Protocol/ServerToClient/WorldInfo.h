#pragma once

#include <Shared/Network/ISerializable.h>
#include <Shared/Network/Archive.h>
#include <Shared/Network/ArchiveConverters.h>
#include <Shared/Geometry/DirectionSet.h>
#include <Shared/Types.hpp>

namespace network {
namespace protocol {

DEFINE_SERIALIZABLE(ObjectInfo, uf::ISerializable)
	uint32_t id;
	std::string name;
	std::vector<uint32_t> spriteIds;
	uint32_t layer;
	uf::Direction direction;
	uf::DirectionSet solidity;
	uf::DirectionSetFractional opacity;

	float moveSpeed;
	uf::vec2f constSpeed;

	void Serialize(uf::Archive &ar) override {
		uf::ISerializable::Serialize(ar);
		ar & id;
		ar & name;
		ar & spriteIds;
		ar & layer;
		ar & direction;
		ar & solidity;
		ar & opacity;
		ar & moveSpeed;
		ar & constSpeed;
	}
DEFINE_SERIALIZABLE_END

DEFINE_SERIALIZABLE(TileInfo, uf::ISerializable)
	uf::vec3i coords;
	uint sprite;
	std::vector<ObjectInfo> content;

	void Serialize(uf::Archive &ar) override {
		uf::ISerializable::Serialize(ar);
		ar & coords;
		ar & sprite;
		ar & content;
	}
DEFINE_SERIALIZABLE_END

} // namespace protocol
} // namespace network
