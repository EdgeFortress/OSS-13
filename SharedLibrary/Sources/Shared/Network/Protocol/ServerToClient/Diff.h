#pragma once

#include <chrono>

#include <Shared/Network/ISerializable.h>
#include <Shared/Network/Protocol/ServerToClient/WorldInfo.h>
#include <Shared/Types.hpp>

namespace network {
namespace protocol {

DEFINE_SERIALIZABLE(Diff, uf::ISerializable)
	uint32_t objId;
	uint32_t invisibility;
	//bool CheckVisibility(uint viewerId, uint visibility) const;

	void Serialize(uf::Archive &ar) override {
		uf::ISerializable::Serialize(ar);
		ar & objId;
	}
DEFINE_SERIALIZABLE_END

DEFINE_SERIALIZABLE(RelocateDiff, Diff)
	ObjectInfo objectInfo;
	uf::vec3i newCoords;
	uint32_t layer;
	void Serialize(uf::Archive &ar) override {
		Diff::Serialize(ar);
		ar & newCoords;
		ar & layer;
	}
DEFINE_SERIALIZABLE_END

DEFINE_PURE_SERIALIZABLE(RelocateAwayDiff, RelocateDiff)

DEFINE_SERIALIZABLE(AddDiff, Diff)
	ObjectInfo objectInfo;
	uf::vec3i coords;
	uint32_t layer;
	void Serialize(uf::Archive &ar) override {
		Diff::Serialize(ar);
		ar & objectInfo;
		ar & coords;
		ar & layer;
	}
DEFINE_SERIALIZABLE_END

DEFINE_PURE_SERIALIZABLE(RemoveDiff, Diff)

DEFINE_SERIALIZABLE(MoveIntentDiff, Diff)
	uf::Direction direction;
	void Serialize(uf::Archive &ar) override {
		Diff::Serialize(ar);
		ar & direction;
	}
DEFINE_SERIALIZABLE_END

DEFINE_SERIALIZABLE(MoveDiff, Diff)
	uf::Direction direction;
	float speed;
	void Serialize(uf::Archive &ar) override {
		Diff::Serialize(ar);
		ar & direction;
		ar & speed;
	}
DEFINE_SERIALIZABLE_END

DEFINE_SERIALIZABLE(UpdateIconsDiff, Diff)
	std::vector<uint32_t> iconsIds;
	void Serialize(uf::Archive &ar) override {
		Diff::Serialize(ar);
		ar & iconsIds;
	}
DEFINE_SERIALIZABLE_END

DEFINE_SERIALIZABLE(PlayAnimationDiff, Diff)
	uint animationId;
	void Serialize(uf::Archive &ar) override {
		Diff::Serialize(ar);
		ar & animationId;
	}
DEFINE_SERIALIZABLE_END

DEFINE_SERIALIZABLE(ChangeDirectionDiff, Diff)
	uf::Direction direction;
	void Serialize(uf::Archive &ar) override {
		Diff::Serialize(ar);
		ar & direction;
	}
DEFINE_SERIALIZABLE_END

DEFINE_SERIALIZABLE(StunnedDiff, Diff)
	std::chrono::microseconds duration;
	void Serialize(uf::Archive &ar) override {
		Diff::Serialize(ar);
		ar & duration;
	}
DEFINE_SERIALIZABLE_END

} // namespace protocol
} // namespace network
