#pragma once

#include <Shared/Network/ISerializable.h>

namespace network {
namespace protocol {

DEFINE_SERIALIZABLE(Diff, uf::ISerializable)
	uint32_t objId;
	uint32_t invisibility;
	//bool CheckVisibility(uint viewerId, uint visibility) const;
DEFINE_SERIALIZABLE_END

DEFINE_SERIALIZABLE(ReplaceDiff, Diff)
	Tile *lastTile;
	ObjectInfo objectInfo;
	int toX, toY, toZ, toObjectNum;
DEFINE_SERIALIZABLE_END

DEFINE_SERIALIZABLE(AddDiff, Diff)
	ObjectInfo objectInfo;
	int toX, toY, toZ, toObjectNum;
DEFINE_SERIALIZABLE_END

DEFINE_SERIALIZABLE(RemoveDiff, Diff)
DEFINE_SERIALIZABLE_END

DEFINE_SERIALIZABLE(MoveIntentDiff, Diff)
	uf::Direction direction;
DEFINE_SERIALIZABLE_END

DEFINE_SERIALIZABLE(MoveDiff, Diff)
	uf::Direction direction;
	float speed;
	Tile *lastblock;
DEFINE_SERIALIZABLE_END

DEFINE_SERIALIZABLE(UpdateIconsDiff, Diff)
	std::vector<IconInfo> icons;
DEFINE_SERIALIZABLE_END

DEFINE_SERIALIZABLE(PlayAnimationDiff, Diff)
	uint animation_id;
DEFINE_SERIALIZABLE_END

DEFINE_SERIALIZABLE(ChangeDirectionDiff, Diff)
	uf::Direction direction;
DEFINE_SERIALIZABLE_END

DEFINE_SERIALIZABLE(StunnedDiff, Diff)
	std::chrono::microseconds duration;
DEFINE_SERIALIZABLE_END

} // namespace protocol
} // namespace network