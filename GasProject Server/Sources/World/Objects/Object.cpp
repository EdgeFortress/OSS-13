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

sf::Vector2f Object::GetShift() const { return shift; }
//float Object::GetSpeed() const { return speed; }

void Object::SetDirection(Global::Direction direction) {
	this->direction = direction;
	if (tile) {
		tile->GetBlock()->AddDiff(new ChangeDirectionDiff(id, direction));
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