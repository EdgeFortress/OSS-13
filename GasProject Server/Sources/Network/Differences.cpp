#include "Differences.hpp"
#include "World/Objects/Object.hpp"

Diff::Diff(const Object *object, Global::DiffType diffType) : 
	diffType(diffType)
{
	id = object->ID();
	invisibility = object->GetInvisibility();
}
Diff::Diff(uint id, uint invisibility, Global::DiffType diffType) :
	diffType(diffType),
	id(id),
	invisibility(invisibility)
{ }
bool Diff::CheckVisibility(uint visibility) const {
	return !(~(~invisibility | visibility)); // if invisible flag then visible flag
}
Global::DiffType Diff::GetType() const { return diffType; }

ReplaceDiff::ReplaceDiff(const Object *object, int toX, int toY, Block *lastBlock) :
	Diff(object, Global::DiffType::RELOCATE),
	lastBlock(lastBlock),
	toX(toX), toY(toY),
	objectInfo(object->GetObjectInfo())
{ }

MoveDiff::MoveDiff(const Object *object, uf::Direction direction, float speed, Block *lastblock) :
	Diff(object, Global::DiffType::MOVE), 
	direction(direction), lastblock(lastblock), speed(speed)
{ }

AddDiff::AddDiff(const Object *object) :
	Diff(object, Global::DiffType::ADD),
	objectInfo(object->GetObjectInfo())
{ }

AddDiff::AddDiff(const ReplaceDiff &replaceDiff) :
	Diff(replaceDiff.id, replaceDiff.invisibility, Global::DiffType::ADD),
	objectInfo(replaceDiff.objectInfo)
{ }

RemoveDiff::RemoveDiff(const Object *object) :
	Diff(object, Global::DiffType::REMOVE)
{ }

ChangeSpriteDiff::ChangeSpriteDiff(const Object *object, uint sprite_id) :
    Diff(object, Global::DiffType::CHANGE_SPRITE),
    sprite_id(sprite_id)
{ }

PlayAnimationDiff::PlayAnimationDiff(const Object *object, uint animation_id) :
    Diff(object, Global::DiffType::PLAY_ANIMATION),
    animation_id(animation_id)
{ }

ChangeDirectionDiff::ChangeDirectionDiff(const Object *object, uf::Direction direction) :
	Diff(object, Global::DiffType::CHANGE_DIRECTION), 
	direction(direction)
{ }