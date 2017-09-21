#pragma once

#include <vector>

#include <SFML/System/Time.hpp>

#include "Map.hpp"
#include "Objects.hpp"

using std::vector;

class World {
private:
    uptr<Map> map;

    Creature *testMob;
    Tile *testMob_lastPosition;
    int test_dx;
    int test_dy;
    int mobsVelocity;

    std::vector<uptr<Object>> objects;

public:
    World();

    void Update(sf::Time timeElapsed);

    void AddObject(Object *);

    void FillingWorld();
    Creature *CreateNewPlayerCreature() const;
};

