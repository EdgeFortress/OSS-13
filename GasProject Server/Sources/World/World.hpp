#pragma once

#include <World/Objects/ObjectHolder.h>
#include <World/Objects/Object.hpp>

using std::vector;

class Map;
class Creature;

class World : public ObjectHolder {
public:
    friend Object;

    World();

    void Update(std::chrono::microseconds timeElapsed);

    void FillingWorld();
	Object *CreateNewPlayerCreature();

    Object *GetObject(uint id) const;
	Map *GetMap() const;

private:
    uptr<Map> map;

	Object *testMob{nullptr};
    Tile *testMob_lastPosition;
    int test_dx;
    int test_dy;
    int mobsVelocity;
};
