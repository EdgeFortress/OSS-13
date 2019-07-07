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
bool Diff::CheckVisibility(uint viewerId, uint visibility) const { // TODO: copypast from object::CheckVisibility - remove it
	return !id                               // viewer is pure camera without object => we can see everything
		|| id == viewerId                    // diff related to viewer itself
		|| !(~(~invisibility | visibility)); // if invisible flag is true, then corresponding visible flag should be true
}
Global::DiffType Diff::GetType() const { return diffType; }

ReplaceDiff::ReplaceDiff(const Object *object, int toX, int toY, int toZ, Tile *lastBlock) :
	Diff(object, Global::DiffType::RELOCATE),
	lastBlock(lastBlock),
	toX(toX), toY(toY), toZ(toZ),
	objectInfo(object->GetObjectInfo())
{ }

ReplaceDiff::ReplaceDiff(const Object *object, int toX, int toY, int toZ) :
	Diff(object, Global::DiffType::RELOCATE_AWAY),
	lastBlock(object->GetTile()),
	toX(toX), toY(toY), toZ(toZ),
	objectInfo(object->GetObjectInfo())
{ }

MoveIntentDiff::MoveIntentDiff(const Object *object, uf::Direction direction) :
    Diff(object, Global::DiffType::MOVE_INTENT),
    direction(direction)
{ }

MoveDiff::MoveDiff(const Object *object, uf::Direction direction, float speed, Tile *lastblock) :
	Diff(object, Global::DiffType::MOVE), 
	direction(direction), lastblock(lastblock), speed(speed)
{ }

AddDiff::AddDiff(const Object *object, int toX, int toY, int toZ) :
	Diff(object, Global::DiffType::ADD),
	toX(toX), toY(toY), toZ(toZ),
	objectInfo(object->GetObjectInfo())
{ }

AddDiff::AddDiff(const ReplaceDiff &replaceDiff) :
	Diff(replaceDiff.id, replaceDiff.invisibility, Global::DiffType::ADD),
	toX(replaceDiff.toX), toY(replaceDiff.toY), toZ(replaceDiff.toZ),
	objectInfo(replaceDiff.objectInfo)
{ }

RemoveDiff::RemoveDiff(const Object *object) :
	Diff(object, Global::DiffType::REMOVE)
{ }

UpdateIconsDiff::UpdateIconsDiff(const Object *object, std::vector<IconInfo> &icons) :
	Diff(object, Global::DiffType::UPDATE_ICONS),
	icons(icons)
{ }

PlayAnimationDiff::PlayAnimationDiff(const Object *object, uint animation_id) :
    Diff(object, Global::DiffType::PLAY_ANIMATION),
    animation_id(animation_id)
{ }

ChangeDirectionDiff::ChangeDirectionDiff(const Object *object, uf::Direction direction) :
	Diff(object, Global::DiffType::CHANGE_DIRECTION), 
	direction(direction)
{ }

StunnedDiff::StunnedDiff(const Object *object, std::chrono::microseconds duration) :
	Diff(object, Global::DiffType::STUNNED),
	duration(duration)
{ }
