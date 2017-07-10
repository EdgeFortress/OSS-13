#include "Object.hpp"

#include "Server.hpp"
#include "World/World.hpp"
#include "Network/TileGrid_Info.hpp"
#include "Network/Differences.hpp"

void Object::takeID() {
    static uint lastID = 0;
    lastID++;
    if (!lastID) Server::log << "Error: object ID overflow" << endl;
    id = lastID;
}

Object::Object() {
    takeID();
    tile = nullptr;
    name = "";
	direction = Global::Direction::NONE;
    CurThreadGame->GetWorld()->AddObject(this);
}

void Object::AddComponent(Component *new_component) {
	new_component->SetOwner(this);
	components.push_back(uptr<Component>(new_component));
}

void Object::Update(sf::Time timeElapsed) {
	for (auto &component : components) {
		component->Update(timeElapsed);
	}
}

bool Object::GetDensity() const { return density; }
Global::Sprite Object::GetSprite() const { return sprite; }

bool Object::CheckVisibility(uint visibility) const { 
	return !(~(~invisibility | visibility)); // if invisible flag then visible flag
}
uint Object::GetInvisibility() const { return invisibility; }

Tile *Object::GetTile() const { return tile; }
std::string Object::GetName() const { return name; }
uint Object::ID() const { return id; }


sf::Vector2f Object::GetShift() const { return shift; }
//float Object::GetSpeed() const { return speed; }

void Object::SetDirection(Global::Direction direction) {
	this->direction = direction;
	if (tile) {
		tile->GetBlock()->AddDiff(new ChangeDirectionDiff(this, direction));
	}
}

void Object::AddShift(sf::Vector2f shift) {
	this->shift += shift;
}

void Object::SetShift(sf::Vector2f shift) {
	this->shift = shift;
}

const ObjectInfo Object::GetObjectInfo() const {
    return std::move(ObjectInfo(id, int(sprite), name, layer, direction, density));
}