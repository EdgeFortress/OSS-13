#include "Object.hpp"

#include "Server.hpp"
#include "World/World.hpp"
#include "Network/TileGrid_Info.hpp"

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
    CurThreadGame->GetWorld()->AddObject(this);
}

void Object::Update(sf::Time timeElapsed) {
	if (speed) {
		if (shift.x) {
			float newShiftX = shift.x - sgn(shift.x) * speed * timeElapsed.asSeconds();
			shift.x = sgn(shift.x) * sgn(newShiftX) > 0 ? newShiftX : 0;
		}
		if (shift.y) {
			float newShiftY = shift.y - sgn(shift.y) * speed * timeElapsed.asSeconds();
			shift.y = sgn(shift.y) * sgn(newShiftY) > 0 ? newShiftY : 0;
		}
	}
}

const ObjectInfo Object::GetObjectInfo() const {
    return std::move(ObjectInfo(id, int(sprite), name, layer, density));
}