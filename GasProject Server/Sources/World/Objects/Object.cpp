#include "Object.hpp"

#include "Shared/TileGrid_Info.hpp"
#include "Shared/Math.hpp"
#include "Shared/Physics/MovePhysics.hpp"

#include "Server.hpp"
#include "World/World.hpp"
#include "Network/Differences.hpp"

Object::Object() :
    density(false), 
    movable(true), 
    layer(0), 
    direction(uf::Direction::NONE), 
    invisibility(0),
    tile(nullptr),
	holder(nullptr),
    moveSpeed(0)
{
    id = CurThreadGame->GetWorld()->addObject(this);
}

void Object::AfterCreation() { }

void Object::Update(sf::Time timeElapsed) {
    for (auto &component : components) {
        component->Update(timeElapsed);
    }

    uf::vec2f deltaShift = uf::phys::countDeltaShift(timeElapsed, shift, moveSpeed, moveIntent, constSpeed, physSpeed);
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
            Tile *dest_tile = tile->GetMap()->GetTile(tile->GetPos() + uf::vec2i(dx, dy));
            if (dest_tile) {
                dest_tile->MoveTo(this);
            }

            if (dx) moveIntent.x = 0;
            if (dy) moveIntent.y = 0;
        }
    }
}

void Object::AddObject(Object *obj) {
	if (!obj) return;

	if (obj->GetTile())
		obj->GetTile()->RemoveObject(obj);

	content.push_back(obj);
	obj->holder = this;
	obj->setTile(GetTile());
}

void Object::AddComponent(Component *new_component) {
	if (!new_component) return;
    new_component->SetOwner(this);
    components.push_back(uptr<Component>(new_component));
}

void Object::SetConstSpeed(uf::vec2f speed) {
    constSpeed = speed;
}

void Object::SetSprite(const std::string &sprite) {
    this->sprite = sprite;
    GetTile()->GetBlock()->AddDiff(new ChangeSpriteDiff(this, Server::Get()->RM->GetSpriteNum(sprite)));
}

void Object::PlayAnimation(const std::string &animation) {
    GetTile()->GetBlock()->AddDiff(new PlayAnimationDiff(this, Server::Get()->RM->GetSpriteNum(animation)));
}

void Object::Delete() {
    if (tile) tile->RemoveObject(this);
    id = 0;
}

uint Object::ID() const { return id; }
std::string Object::GetName() const { return name; }
Tile *Object::GetTile() const { return tile; }

bool Object::GetDensity() const { return density; };
bool Object::IsMovable() const { return movable; };
bool Object::IsCloseTo(Object *other) const {
    auto pos = GetTile()->GetPos();
    auto otherPos = other->GetTile()->GetPos();
    if (uf::length(pos - otherPos) < 2)
        return true;
	return false;
}

uint Object::GetInvisibility() const { return invisibility; }
bool Object::CheckVisibility(uint visibility) const {
    return !(~(~invisibility | visibility)); // if invisible flag then visible flag
}

//Global::Sprite Object::GetSprite() const { return sprite; }
uint Object::GetLayer() const { return layer; }


void Object::SetMoveIntent(uf::vec2i moveIntent) {
    if (tile) {
        tile->GetBlock()->AddDiff(new MoveIntentDiff(this, uf::VectToDirection(moveIntent)));
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
//float Object::GetSpeed() const { 
//    if (constSpeed) {
//        return constSpeed.y;
//    }
//    return speed; 
//}

void Object::SetDirection(uf::Direction direction) {
    if (direction > uf::Direction::EAST)
        direction = uf::Direction(char(direction) % 4);
    this->direction = direction;
	if (tile)
		tile->GetBlock()->AddDiff(new ChangeDirectionDiff(this, direction));
}

//void Object::AddShift(uf::vec2f shift) {
//    delta_shift += shift;
//}

const ObjectInfo Object::GetObjectInfo() const {
    ObjectInfo objectInfo(id, Server::Get()->RM->GetSpriteNum(sprite), name, layer, direction, density);
    objectInfo.constSpeed = constSpeed;
    objectInfo.moveSpeed = moveSpeed;
    return objectInfo;
}

void Object::setTile(Tile *newTile) {
	tile = newTile;
	for (auto *obj: content)
		obj->setTile(newTile);
}
