#include "Object.hpp"

#include "Server.hpp"
#include "World/World.hpp"
#include "Shared/TileGrid_Info.hpp"
#include "Network/Differences.hpp"

Object::Object() {
    takeID();
    tile = nullptr;
    name = "";
	direction = uf::Direction::NONE;
    movable = true;
    CurThreadGame->GetWorld()->AddObject(this);
}

void Object::Update(sf::Time timeElapsed) {
    for (auto &component : components) {
        component->Update(timeElapsed);
    }
}

void Object::AddComponent(Component *new_component) {
    new_component->SetOwner(this);
    components.push_back(uptr<Component>(new_component));
}

uint Object::ID() const { return id; }
std::string Object::GetName() const { return name; }
Tile *Object::GetTile() const { return tile; }

bool Object::GetDensity() const { return density; }
bool Object::IsMovable() const { return movable; }
uint Object::GetInvisibility() const { return invisibility; }
bool Object::CheckVisibility(uint visibility) const {
    return !(~(~invisibility | visibility)); // if invisible flag then visible flag
}

//Global::Sprite Object::GetSprite() const { return sprite; }
uint Object::GetLayer() const { return layer; }


uf::vec2f Object::GetShift() const { return shift; }
//float Object::GetSpeed() const { return speed; }

void Object::SetDirection(uf::Direction direction) {
	this->direction = direction;
	if (tile) {
		tile->GetBlock()->AddDiff(new ChangeDirectionDiff(this, direction));
	}
}

void Object::AddShift(uf::vec2f shift) {
	this->shift += shift;
}

void Object::SetShift(uf::vec2f shift) {
	this->shift = shift;
}

const ObjectInfo Object::GetObjectInfo() const {
    return std::move(ObjectInfo(id, Server::Get()->RM->GetIconNum(sprite), name, layer, direction, density));
}

void Object::takeID() {
    static uint lastID = 0;
    lastID++;
    if (!lastID) Server::log << "Error: object ID overflow" << std::endl;
    id = lastID;
}