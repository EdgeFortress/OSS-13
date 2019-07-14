#include "Object.hpp"

#include <plog/Log.h>

#include <IServer.h>
#include <IGame.h>
#include <Resources/ResourceManager.hpp>
#include <Network/Differences.hpp>
#include <World/World.hpp>
#include <World/Map.hpp>
#include <World/Objects/Control.hpp>

#include <Shared/TileGrid_Info.hpp>
#include <Shared/Math.hpp>
#include <Shared/Physics/MovePhysics.hpp>

Object::Object() :
    density(false), 
    movable(true),
	spriteState(Global::ItemSpriteState::DEFAULT),
    layer(0), 
    direction(uf::Direction::NONE), 
    invisibility(0),
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

	uf::vec2f deltaShift = uf::phys::countDeltaShift(sf::microseconds(timeElapsed.count()), shift, moveSpeed, moveIntent, constSpeed, physSpeed);
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
		GetTile()->AddDiff(new UpdateIconsDiff(this, icons));
		iconsOutdated = false;
	}

	animationTimer.Update(timeElapsed);
}

void Object::Move(uf::vec2i order) {
	if (!order)
		return;

	// Form the intent based on the order
	SetDirection(uf::VectToDirection(order));

	Tile *tile = GetTile();
	if (tile) {
		uf::vec2i moveIntent;

		if (order.x) moveIntent.x = order.x;
		if (order.y) moveIntent.y = order.y;

		Tile *newTileDiag = tile->GetMap()->GetTile(tile->GetPos() + rpos(moveIntent, 0));
		Tile *newTileX = tile->GetMap()->GetTile({ tile->GetPos().x + moveIntent.x, tile->GetPos().y, tile->GetPos().z });
		Tile *newTileY = tile->GetMap()->GetTile({ tile->GetPos().x, tile->GetPos().y + moveIntent.y, tile->GetPos().z });

		if (GetDensity()) {
			if (!newTileDiag || newTileDiag->IsDense()) moveIntent = GetMoveIntent();
			if (!newTileX || newTileX->IsDense()) moveIntent.x = 0;
			if (!newTileY || newTileY->IsDense()) moveIntent.y = 0;
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

void Object::SetConstSpeed(uf::vec2f speed) {
    constSpeed = speed;
}

void Object::Delete() {
    if (tile) tile->RemoveObject(this);
    id = 0;
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

	GetTile()->AddDiff(new PlayAnimationDiff(this, iconInfo.id));

	animationTimer.Start(iconInfo.animation_time, std::forward<std::function<void()>>(callback));
	return true;
}

bool Object::GetDensity() const { return density; };
void Object::SetDensity(bool density) { this->density = density; }

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
        tile->AddDiff(new MoveIntentDiff(this, uf::VectToDirection(moveIntent)));
    }
    if (moveIntent.x) this->moveIntent.x = moveIntent.x;
    if (moveIntent.y) this->moveIntent.y = moveIntent.y;
}

uf::vec2i Object::GetMoveIntent() const {
    return moveIntent;
}

void Object::SetMoveSpeed(float speed) {
    this->moveSpeed = speed;
}

float Object::GetMoveSpeed() const {
    return moveSpeed;
}

//uf::vec2f Object::GetShift() const { return shift + delta_shift; }
float Object::GetSpeed() const {
	if (constSpeed) {
		return uf::abs(constSpeed.y);
	}
	return moveSpeed;
}

void Object::SetDirection(uf::Direction direction) {
    if (direction > uf::Direction::EAST)
        direction = uf::Direction(char(direction) % 4);
    this->direction = direction;
	if (tile)
		tile->AddDiff(new ChangeDirectionDiff(this, direction));
}

//void Object::AddShift(uf::vec2f shift) {
//    delta_shift += shift;
//}

bool Object::IsFloor() const { return isFloor; }
void Object::SetIsFloor(bool value) { isFloor = value; }

bool Object::IsWall() const { return isWall; }
void Object::SetIsWall(bool value) { isWall = value; }

ObjectInfo Object::GetObjectInfo() const {
    ObjectInfo objectInfo;
	objectInfo.id = id;
	objectInfo.name = name;
	objectInfo.layer = layer;
	objectInfo.direction = direction;
	objectInfo.dense = density;
    objectInfo.constSpeed = constSpeed;
    objectInfo.moveSpeed = moveSpeed;

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
