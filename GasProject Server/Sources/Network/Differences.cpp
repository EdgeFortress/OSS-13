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

MoveIntentDiff::MoveIntentDiff(const Object *object, uf::Direction direction) :
    Diff(object, Global::DiffType::MOVE_INTENT),
    direction(direction)
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

UpdateIconsDiff::UpdateIconsDiff(const Object *object, std::vector<uint32_t> &icons) :
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

StunnedDiff::StunnedDiff(const Object *object, sf::Time duration) :
	Diff(object, Global::DiffType::STUNNED),
	duration(duration)
{ }
