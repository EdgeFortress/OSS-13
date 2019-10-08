#pragma once

#include <chrono>

#include <Shared/Network/ISerializable.h>
#include <Shared/Network/Protocol/ServerToClient/WorldInfo.h>
#include <Shared/Network/Syncable/ObjectSyncFields.h>
#include <Shared/Types.hpp>

namespace network {
namespace protocol {

DEFINE_SERIALIZABLE(Diff, uf::ISerializable)
	uint32_t objId;

	void Serialize(uf::Archive &ar) override {
		uf::ISerializable::Serialize(ar);
		ar & objId;
	}

	// Server side infrastructure
	Diff() { diffId = ++diffCounter; }
	static void ResetDiffCounter() { diffCounter = 0; }
	uint32_t GetDiffId() { return diffId; }

private:
	uint32_t diffId;
	static uint32_t diffCounter;
DEFINE_SERIALIZABLE_END

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
	
DEFINE_SERIALIZABLE(FieldsDiff, Diff)
	uf::SyncableChanges fieldsChanges;

	void Serialize(uf::Archive &ar) override {
		Diff::Serialize(ar);
		ar & fieldsChanges;
	}
DEFINE_SERIALIZABLE_END

DEFINE_SERIALIZABLE(RelocateDiff, Diff)
	uf::vec3i newCoords;
	uint32_t layer;
	void Serialize(uf::Archive &ar) override {
		Diff::Serialize(ar);
		ar & newCoords;
		ar & layer;
	}
DEFINE_SERIALIZABLE_END

DEFINE_PURE_SERIALIZABLE(RelocateAwayDiff, RelocateDiff)

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
