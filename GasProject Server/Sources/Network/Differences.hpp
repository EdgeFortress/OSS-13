#pragma once

#include <string>

#include <SFML/System/Time.hpp>

#include <Resources/IconInfo.h>

#include <Shared/Types.hpp>
#include <Shared/Global.hpp>
#include <Shared/TileGrid_Info.hpp>

class Tile;
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
    Tile *lastBlock;
	ObjectInfo objectInfo;
    int toX, toY, toObjectNum;

	ReplaceDiff(const Object *object, int toX, int toY, Tile* lastBlock);
};

struct AddDiff : public Diff {
	ObjectInfo objectInfo;
	explicit AddDiff(const Object *object);
	explicit AddDiff(const ReplaceDiff &replaceDiff);
};

struct RemoveDiff : public Diff {
	RemoveDiff(const Object *object);
};

struct MoveIntentDiff : public Diff {
    uf::Direction direction;

    MoveIntentDiff(const Object *object, uf::Direction direction);
};

struct MoveDiff : public Diff {
    uf::Direction direction;
	float speed;
    Tile *lastblock;

	MoveDiff(const Object *object, uf::Direction direction, float speed, Tile* lastblock);
};

struct UpdateIconsDiff : public Diff {
	std::vector<IconInfo> icons;

	UpdateIconsDiff(const Object *object, std::vector<IconInfo> &icons);
};

struct PlayAnimationDiff : public Diff {
    uint animation_id;

    PlayAnimationDiff(const Object *object, uint animation_id);
};

struct ChangeDirectionDiff : public Diff {
	uf::Direction direction;

	ChangeDirectionDiff(const Object *object, uf::Direction direction);
};

struct StunnedDiff : public Diff {
	sf::Time duration;

	StunnedDiff(const Object *object, sf::Time duration);
};
