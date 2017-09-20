#pragma once

#include <string>

#include "Shared/Types.hpp"
#include "Shared/Global.hpp"
#include "Shared/TileGrid_Info.hpp"

class Block;
class Object;

struct Diff {
    uint id;
	uint invisibility;

	virtual Global::DiffType GetType() const final;
	bool CheckVisibility(uint visibility) const;
protected:
	Diff(const Object *object, Global::DiffType diffType = Global::DiffType::NONE);
	Diff(uint id, uint invisibility, Global::DiffType diffType);
private:
	Global::DiffType diffType;
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
    uf::Direction direction;
	float speed;
    Block *lastblock;

	MoveDiff(const Object *object, uf::Direction direction, float speed, Block *lastblock);
};

struct ChangeDirectionDiff : public Diff {
	uf::Direction direction;

	ChangeDirectionDiff(const Object *object, uf::Direction direction);
};