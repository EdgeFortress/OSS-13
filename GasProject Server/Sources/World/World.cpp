#include "World.hpp"

#include "Server.hpp"
#include "Map.hpp"
#include "Tile.hpp"
#include "Objects.hpp"
#include "Objects/Control.hpp"
#include "Player.hpp"

World::World() : map(new Map(100, 100)) {
    
}

void World::Update(sf::Time timeElapsed) {
    map->ClearDiffs();

	// Simple walking mob AI for moving testing
    if (testMob_lastPosition != testMob->GetTile()) {
		testMob_lastPosition = testMob->GetTile();
        const int x = testMob->GetTile()->GetPos().x + test_dx;
        const int y = testMob->GetTile()->GetPos().y + test_dy;
        if (test_dx == 1 && x == 53) test_dx = 0, test_dy = 1;
        if (test_dy == 1 && y == 53) test_dx = -1, test_dy = 0;
        if (test_dx == -1 && x == 47) test_dx = 0, test_dy = -1;
        if (test_dy == -1 && y == 47) test_dx = 1, test_dy = 0;

		testMob->GetComponent<Control>()->MoveCommand(sf::Vector2i(test_dx, test_dy));
    }
    
    map->Update(timeElapsed);

    // update objects
    for (uint i = 0; i < objects.size(); i++) {
        if (!objects[i].get()) continue; // already deleted
        if (!objects[i]->ID()) {         // waiting for delete
            objects[i].release();
            free_ids.push_back(i + 1);
            continue;
        }
        objects[i]->Update(timeElapsed);
    }
}

void World::FillingWorld() {
    for (int i = 45; i <= 55; i++) {
        for (int j = 45; j <= 55; j++) {
            Tile *tile = map->GetTile({ i, j });
            tile->PlaceTo(new Floor);
            if (i == 45 || i == 55 || j == 45 || j == 55) {
                if (i == 50 || j == 50) {
                    Airlock *airlock = new Airlock;
                    tile->PlaceTo(airlock);
                    if (i == 55 && j == 50 || i == 50 && j == 55)
                        airlock->Lock();
                }
                else tile->PlaceTo(new Wall);
            }
        }
    }

    for (int i = 5; i <= 10; i++) {
        for (int j = 5; j <= 10; j++) {
            map->GetTile({ i, j })->PlaceTo(new Floor);
        }
    }

    testMob = new Ghost;
	testMob_lastPosition = nullptr;
    map->GetTile({ 49, 49 })->PlaceTo(testMob);
    test_dx = 1;
    test_dy = 0;

    for (int i = 85; i <= 95; i++) {
        for (int j = 85; j <= 95; j++) {
            Tile *tile = map->GetTile({ i, j });
            tile->PlaceTo(new Floor);
        }
    }
}

Creature *World::CreateNewPlayerCreature() const {
    Creature *creature = new Human();
    map->GetTile({ 50, 50 })->PlaceTo(creature);
    return creature;
}

Object *World::GetObject(uint id) {
    if (id <= objects.size()) {
        Object *object = objects[id - 1].get();
        if (!object || !object->ID())
            return nullptr; // object deleted
        return object;
    }
    return nullptr;
}

uint World::addObject(Object *obj) {
    if (free_ids.empty()) {
        objects.push_back(uptr<Object>(obj));
        return uint(objects.size());
    } else {
        uint id = free_ids.back();
        free_ids.pop_back();
        objects[id - 1] = uptr<Object>(obj);
        return id;
    }
}
