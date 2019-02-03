#pragma once

#include <World/Objects/ObjectHolder.h>

using std::vector;

class Map;
class Creature;

class World : public ObjectHolder {
public:
    friend Object;

    World();

    void Update(sf::Time timeElapsed);

    void FillingWorld();
    Creature *CreateNewPlayerCreature();

    Object *GetObject(uint id) const;
	Map *GetMap() const;

private:
    uptr<Map> map;

    Creature *testMob;
    Tile *testMob_lastPosition;
    int test_dx;
    int test_dy;
    int mobsVelocity;
};
