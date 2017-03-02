#pragma once

#include <string>
#include <SFML/System/Vector2.hpp>
#include <SFML/System/Time.hpp>

#include "Common/NetworkConst.hpp"
#include "Common/Useful.hpp"

class Tile;
struct ObjectInfo;

namespace sf {
    class Packet;
}

class Object {
private:
    void takeID();

protected:
    uint id;
    Tile *tile;
    std::string name;
    Global::Sprite sprite;
    uint layer;
    bool density;

	sf::Vector2f shift;
	float speed; // speed (tiles/seconds)

public:
    Object();

    virtual void Update(sf::Time timeElapsed);

    bool GetDensity() const { return density; }
    Global::Sprite GetSprite() const { return sprite; }
    Tile *GetTile() const { return tile; }
    std::string GetName() const { return name; }
    uint ID() const { return id; }

    //// Just set tile pointer
    //void SetTile(Tile *tile) { this->tile = tile; }

    virtual void Interact(Object *) = 0;

    const ObjectInfo GetObjectInfo() const;

    friend Tile;
    //friend sf::Packet &operator<<(sf::Packet &, const Object &);
};