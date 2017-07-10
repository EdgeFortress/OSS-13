#pragma once

#include <string>

#include "Common/NetworkConst.hpp"
#include "Common/Useful.hpp"
#include "TileGrid_Info.hpp"

class Block;
class Object;

struct Diff {
	Global::DiffType diffType;
    uint id;
	uint invisibility;

	Diff(const Object *object, Global::DiffType diffType = Global::DiffType::NONE);
	Diff(uint id, uint invisibility, Global::DiffType diffType);

	virtual Global::DiffType GetType() const;
	bool CheckVisibility(uint visibility) const;
};

struct ReplaceDiff : public Diff {
    Block *lastBlock;
	ObjectInfo objectInfo;
    int toX, toY, toObjectNum;

	ReplaceDiff(const Object *object, int toX, int toY, Block *lastBlock);
};

struct AddDiff : public Diff {
	ObjectInfo objectInfo;
	explicit AddDiff(const Object *object);
	explicit AddDiff(const ReplaceDiff &replaceDiff);
};

struct RemoveDiff : public Diff {
	RemoveDiff(const Object *object);
};

struct MoveDiff : public Diff {
    Global::Direction direction;
	float speed;
    Block *lastblock;

	MoveDiff(const Object *object, Global::Direction direction, float speed, Block *lastblock);
};

struct ChangeDirectionDiff : public Diff {
	Global::Direction direction;

	ChangeDirectionDiff(const Object *object, Global::Direction direction);
};