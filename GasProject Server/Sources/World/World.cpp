#include "World.hpp"

#include "Server.hpp"
#include "Map.hpp"
#include "Tile.hpp"
#include "Objects.hpp"
#include "Objects/Control.hpp"
#include "Player.hpp"

World::World() : 
	map(new Map(100, 100))
{ }

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

		//testMob->GetComponent<Control>()->MoveCommand(sf::Vector2i(test_dx, test_dy));
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
			CreateObject<Floor>({ i, j });
            if (i == 45 || i == 55 || j == 45 || j == 55) {
                if (i == 50 || j == 50) {
					auto airlock = CreateObject<Airlock>({ i, j });
                    if (i == 55 && j == 50 || i == 50 && j == 55)
                        airlock->Lock();
                }
                else CreateObject<Wall>({ i, j });
            }
        }
    }

	CreateObject<Taser>({ 50, 50 });
	CreateObject<Taser>({ 55, 50 });
	CreateObject<Taser>({ 52, 50 });
	CreateObject<Uniform>({ 49, 50 });

    for (int i = 5; i <= 10; i++) {
        for (int j = 5; j <= 10; j++) {
			CreateObject<Floor>({ i, j });
        }
    }

	testMob = CreateObject<Ghost>({ 49, 49 });
	testMob_lastPosition = nullptr;

    test_dx = 1;
    test_dy = 0;

    for (int i = 85; i <= 95; i++) {
        for (int j = 85; j <= 95; j++) {
			CreateObject<Floor>({ i, j });
        }
    }
}

Creature *World::CreateNewPlayerCreature() {
	auto human = CreateObject<Human>();

	auto uniform = CreateObject<Uniform>();
	human->PutOn(uniform);

    map->GetTile({ 50, 50 })->PlaceTo(human);

    return human;
}

Object *World::GetObject(uint id) const {
    if (id <= objects.size()) {
        Object *object = objects[id - 1].get();
        if (!object || !object->ID())
            return nullptr; // object deleted
        return object;
    }
    return nullptr;
}

Map *World::GetMap() const {
	return map.get();
}
