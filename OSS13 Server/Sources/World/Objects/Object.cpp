#include "Object.hpp"

#include <plog/Log.h>

#include <IServer.h>
#include <IGame.h>
#include <Resources/ResourceManager.hpp>
#include <World/World.hpp>
#include <World/Map.hpp>
#include <World/Objects/Control.hpp>

#include <Shared/Math.hpp>
#include <Shared/Physics/MovePhysics.hpp>
#include <Shared/Network/Protocol/ServerToClient/Diff.h>

Object::Object() :
    movable(true),
	spriteState(Global::ItemSpriteState::DEFAULT),
    layer(0), 
    direction(uf::Direction::NONE),
    tile(nullptr),
	holder(nullptr),
    moveSpeed(0)
{ }

void Object::Update(std::chrono::microseconds timeElapsed) {
	if (!GetTile())
		return;

	for (auto &idAndComponent : components) {
		idAndComponent.second->Update(timeElapsed);
	}

	uf::vec2f deltaShift = uf::phys::countDeltaShift(sf::microseconds(timeElapsed.count()), shift, moveSpeed, moveIntent, speed);
	shift += deltaShift;

	if (shift) {
		int dx, dy;
		dy = dx = 0;
		uf::vec2f shiftChange;
		if (abs(shift.x) >= 0.5f) {
			dx += int(uf::sgn(shift.x) * floor(abs(shift.x) - 0.5f + 1.f));
			shiftChange.x -= dx;
		}
		if (abs(shift.y) >= 0.5f) {
			dy += int(uf::sgn(shift.y) * floor(abs(shift.y) - 0.5f + 1.f));
			shiftChange.y -= dy;
		}
		shift += shiftChange;

		if (dx || dy) {
			Tile *tile = GetTile();
			Tile *dest_tile = tile->GetMap()->GetTile(tile->GetPos() + rpos(dx, dy, 0));
			if (dest_tile) {
				dest_tile->MoveTo(this);
			}

			if (dx) moveIntent.x = 0;
			if (dy) moveIntent.y = 0;
		}
	}

	if (iconsOutdated) {
		updateIcons();
		auto diff = std::make_shared<network::protocol::UpdateIconsDiff>();
		diff->objId = ID();
		for (auto &iconInfo : icons)
			diff->iconsIds.push_back(iconInfo.id + static_cast<uint32_t>(iconInfo.state));
		GetTile()->AddDiff(diff, this);
		iconsOutdated = false;
	}

	animationTimer.Update(timeElapsed);
}

void Object::Move(uf::vec2i order) {
	if (!order)
		return;

	SetDirection(uf::VectToDirection(order));

	Tile *tile = GetTile();
	if (tile) {
		uf::vec2i moveIntent = GetMoveIntent();

		if (order.x) moveIntent.x = order.x;
		if (order.y) moveIntent.y = order.y;

		Tile *newTileX = tile->GetMap()->GetTile({ tile->GetPos().x + moveIntent.x, tile->GetPos().y, tile->GetPos().z });
		Direction xDirection = newTileX ? uf::VectToDirection(newTileX->GetPos() - tile->GetPos()) : uf::Direction::NONE;

		Tile *newTileY = tile->GetMap()->GetTile({ tile->GetPos().x, tile->GetPos().y + moveIntent.y, tile->GetPos().z });
		Direction yDirection = newTileY ? uf::VectToDirection(newTileY->GetPos() - tile->GetPos()) : uf::Direction::NONE;

		Tile *newTileDiag = tile->GetMap()->GetTile(tile->GetPos() + rpos(moveIntent, 0));

		if (GetDensity()) {
			auto moveDirection = uf::VectToDirection(moveIntent);

			if (tile->IsDense({moveDirection})) { // exit from current tile
				moveIntent = GetMoveIntent();
			} else {
				if (!newTileDiag || newTileDiag->IsDense({uf::InvertDirection(moveDirection), uf::Direction::CENTER})) {
					return;
				}
				else {
					if (!newTileX || newTileX != tile && newTileX->IsDense({ uf::InvertDirection(xDirection), yDirection, uf::Direction::CENTER })) return;
					if (!newTileY || newTileY != tile && newTileY->IsDense({ uf::InvertDirection(yDirection), xDirection, uf::Direction::CENTER })) return;
				}
			}
		}

		SetMoveIntent(moveIntent);
	}
}

void Object::AddComponent(Component *new_component) {
	if (!new_component) return;
	new_component->SetOwner(this);
	components[new_component->ID()] = uptr<Component>(new_component);
}

void Object::AddComponent(const std::string &id) {
	uptr<Component> component;
	if (id == "Control") {
		component.reset(new Control());	
	}

	if (component) {
		component->SetOwner(this);
		components[component->ID()] = std::move(component);
	}
}

Component *Object::GetComponent(const std::string &id) {
	return components[id].get();
}

void Object::AddObject(Object *obj) {
	if (!obj) return;

	if (obj->GetHolder()) {
        if (!obj->GetHolder()->RemoveObject(obj))
            return;
    } else if (obj->GetTile())
		obj->GetTile()->RemoveObject(obj);

	content.push_back(obj);
	obj->holder = this;
	obj->setTile(GetTile());

	askToUpdateIcons();
}

bool Object::RemoveObject(Object *obj) {
    for (auto iter = content.begin(); iter != content.end(); iter++) {
        if (*iter == obj) {
            obj->setTile(nullptr);
            obj->holder = nullptr;
            content.erase(iter);
			askToUpdateIcons();
            return true;
        }
    }
    return false;
}

void Object::Delete() {
    if (tile) tile->RemoveObject(this);
    markedToBeDeleted = true;
}

uint Object::ID() const { return id; }

const std::string &Object::GetName() const { return name; }
void Object::SetName(const std::string& name) { this->name = name; };

const std::string &Object::GetSprite() const { return sprite; }
void Object::SetSprite(const std::string &sprite) {
	this->sprite = sprite;
	askToUpdateIcons();
}

uint Object::GetLayer() const { return layer; }
void Object::SetLayer(uint layer) { this->layer = layer; }

void Object::SetSpriteState(Global::ItemSpriteState newState) {
	spriteState = newState;
}

bool Object::PlayAnimation(const std::string &animation, std::function<void()> callback) {
	if (!animationTimer.IsStopped())
		return false;

	auto iconInfo = IServer::RM()->GetIconInfo(animation);

	auto playAnimationDiff = std::make_shared<network::protocol::PlayAnimationDiff>();
	playAnimationDiff->objId = ID();
	playAnimationDiff->animationId = iconInfo.id;

	GetTile()->AddDiff(std::move(playAnimationDiff), this);

	animationTimer.Start(iconInfo.animation_time, std::forward<std::function<void()>>(callback));
	return true;
}

bool Object::GetDensity() const { return solidity.IsExistsOne({Direction::CENTER}); };
void Object::SetDensity(bool density) { density ? solidity.Add({Direction::CENTER}) : solidity.Remove({Direction::CENTER}); }

void Object::SetSolidity(uf::DirectionSet directions) { solidity = directions; }
const uf::DirectionSet &Object::GetSolidity() const { return solidity; }

void Object::SetOpacity(uf::DirectionSetFractional fractionalDirections) { opacity = fractionalDirections; }
const uf::DirectionSetFractional &Object::GetOpacity() const { return opacity; };

void Object::SetAirtightness(uf::DirectionSetFractional fractionalDirections) {airtightness = fractionalDirections; }
const uf::DirectionSetFractional &Object::GetAirtightness() const { return airtightness; }

void Object::SetPosition(uf::vec2i newPos) {
	if (newPos > uf::vec2i(0, 0)) {
		auto tile = GGame->GetWorld()->GetMap()->GetTile(apos(newPos));
		if (tile) {
			tile->PlaceTo(this);
			return;
		}
	}
	// incorrect newPos
	if (this->tile)
		this->tile->RemoveObject(this);
}

uf::vec2i Object::GetPosition() const {
	auto tile = GetTile();
	if (tile)
		return uf::vec2i(tile->GetPos().xy());
	else
		return {-1, -1};
}

Tile *Object::GetTile() const { return tile; }
void Object::SetTile(Tile *tile) {
	if (!tile) {
		if (this->tile)
			this->tile->RemoveObject(this);
	}
	tile->PlaceTo(this);
}

void Object::SetMoveSpeed(float speed) { this->moveSpeed = speed; }
float Object::GetMoveSpeed() const { return moveSpeed; }

void Object::SetSpeed(uf::vec2f speed) { this->speed = speed; }
uf::vec2f Object::GetSpeed() const { return speed; }

Object *Object::GetHolder() const { return holder; }

bool Object::IsMovable() const { return movable; };
bool Object::IsCloseTo(Object *other) const {
    auto pos = GetTile()->GetPos();
    auto otherPos = other->GetTile()->GetPos();
    if (uf::length(pos - otherPos) < 2)
        return true;
	return false;
}

bool Object::CheckVisibility(uint viewerId, uint visibility) const {
	return !id                               // viewer is pure camera without object => we can see everything
		|| id == viewerId                    // diff related to viewer itself
		|| !(~(~invisibility | visibility)); // if invisible flag is true, then corresponding visible flag should be true
}

uint Object::GetInvisibility() const { return invisibility; }
void Object::SetInvisibility(uint invisibility) { this->invisibility = invisibility; }


void Object::SetMoveIntent(uf::vec2i moveIntent) {
	if (tile) {
		auto moveIntentDiff = std::make_shared<network::protocol::MoveIntentDiff>();
		moveIntentDiff->objId = ID();
		moveIntentDiff->direction = uf::VectToDirection(moveIntent);
		tile->AddDiff(std::move(moveIntentDiff), this);
	}
	if (moveIntent.x) this->moveIntent.x = moveIntent.x;
	if (moveIntent.y) this->moveIntent.y = moveIntent.y;
}

uf::vec2i Object::GetMoveIntent() const {
    return moveIntent;
}

void Object::SetDirection(uf::Direction direction) {
    if (direction > uf::Direction::EAST)
        direction = uf::Direction(char(direction) % 4);
    this->direction = direction;
	if (tile) {
		auto changeDirectionDiff = std::make_shared<network::protocol::ChangeDirectionDiff>();
		changeDirectionDiff->objId = ID();
		changeDirectionDiff->direction = direction;
		tile->AddDiff(std::move(changeDirectionDiff), this);
	}
}

//void Object::AddShift(uf::vec2f shift) {
//    delta_shift += shift;
//}

bool Object::IsFloor() const { return isFloor; }
void Object::SetIsFloor(bool value) { isFloor = value; }

bool Object::IsWall() const { return isWall; }
void Object::SetIsWall(bool value) { isWall = value; }

network::protocol::ObjectInfo Object::GetObjectInfo() const {
	network::protocol::ObjectInfo objectInfo;
	objectInfo.id = id;
	objectInfo.name = name;
	objectInfo.layer = layer;
	objectInfo.direction = direction;
	objectInfo.solidity = solidity;
	objectInfo.opacity = opacity;
	objectInfo.moveSpeed = moveSpeed;
	objectInfo.speed = speed;

	for (auto &iconInfo : icons)
		objectInfo.spriteIds.push_back(iconInfo.id + static_cast<uint32_t>(iconInfo.state));

    return objectInfo;
}

void Object::updateIcons() {
	icons.clear();
	icons.push_back(IServer::RM()->GetIconInfo(sprite));
}

void Object::pushToIcons(const IconInfo &icon) {
	icons.push_back(icon);
}

void Object::askToUpdateIcons() {
	iconsOutdated = true;
}

void Object::setTile(Tile *newTile) {
	tile = newTile;
	for (auto *obj: content)
		obj->setTile(newTile);
}
