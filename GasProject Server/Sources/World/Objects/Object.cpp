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

const ObjectInfo Object::GetObjectInfo() const {
    return std::move(ObjectInfo(id, int(sprite), name, layer));
}