#pragma once

#include <string>

#include "Common/NetworkConst.hpp"

class Tile;
struct ObjectInfo;

namespace sf {
    class Packet;
}

class Object {
protected:
    Tile *tile;
    std::string name;
    Global::Sprite sprite;
    bool density;

public:
    Object();

    virtual void Update() = 0;

    bool GetDensity() const { return density; }
    Global::Sprite GetSprite() const { return sprite; }
    Tile *GetTile() const { return tile; }
    std::string GetName() const { return name; }

    // Just set tile pointer
    void SetTile(Tile *tile) { this->tile = tile; }

    virtual void Interact(Object *) = 0;

    const ObjectInfo GetObjectInfo() const;

    friend sf::Packet &operator<<(sf::Packet &, const Object &);
};