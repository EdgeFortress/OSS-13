#pragma once

#include <vector>

#include <SFML/System/Time.hpp>

#include "Map.hpp"
#include "Objects.hpp"

using std::vector;

class World {
public:
    World();

    void Update(sf::Time timeElapsed);

    void AddObject(Object *);

    void FillingWorld();
    Creature *CreateNewPlayerCreature() const;

private:
    uptr<Map> map;

    Creature *testMob;
    Tile *testMob_lastPosition;
    int test_dx;
    int test_dy;
    int mobsVelocity;

    std::vector<uptr<Object>> objects;
};

//class Local {
//    list<Tile *> tiles;
//    Map *map;
//
//public:
//    Local(Tile *tile) : map(tile->GetMap()) { tiles.push_back(tile); }
//
//    void AddTile(Tile *tile) {
//        tiles.push_back(tile);
//    }
//    void Merge(Local *local) {
//        if (!local) return;
//        if (this == local) return;
//        for (auto &tile : local->tiles) {
//            tiles.push_back(tile);
//            tile->SetLocal(this);
//        }
//        map->RemoveLocal(local);
//    }
//    test
//    const list<Tile *> &GetTiles() { return tiles; }
//};
//
//class Gas {
//    double pressure;
//};
//
//class Oxygen : public Gas {
//
//};
