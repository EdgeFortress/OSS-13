#include "Object.hpp"

#include "Server.hpp"
#include "World/World.hpp"
#include "Network/TileGrid_Info.hpp"

Object::Object() {
    tile = nullptr;
    name = "";
    CurThreadGame->GetWorld()->AddObject(this);
}

const ObjectInfo Object::GetObjectInfo() const {
    return std::move(ObjectInfo(int(sprite), name));
}