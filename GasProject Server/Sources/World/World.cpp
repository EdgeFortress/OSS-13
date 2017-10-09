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
        const int x = testMob->GetTile()->X() + test_dx;
        const int y = testMob->GetTile()->Y() + test_dy;
        if (test_dx == 1 && x == 53) test_dx = 0, test_dy = 1;
        if (test_dy == 1 && y == 53) test_dx = -1, test_dy = 0;
        if (test_dx == -1 && x == 47) test_dx = 0, test_dy = -1;
        if (test_dy == -1 && y == 47) test_dx = 1, test_dy = 0;

		testMob->GetComponent<Control>()->MoveCommand(sf::Vector2i(test_dx, test_dy));
    }
    
    map->Update(timeElapsed);

    for (unsigned i = 0; i < objects.size();) {
        const auto len = objects.size();
		Object *object = (objects.begin() + i)->get();
		if (object->GetTile())
			object->Update(timeElapsed);
		else
			objects.erase(objects.begin() + i);
		int delta = int(objects.size() - len + 1);
		if (delta < 0) delta = 0;
		i += delta;
    }
}

void World::FillingWorld() {
    for (int i = 45; i <= 55; i++) {
        for (int j = 45; j <= 55; j++) {
            Tile *tile = map->GetTile(i, j);
            tile->PlaceTo(new Floor);
            if (i == 45 || i == 55 || j == 45 || j == 55)
                tile->PlaceTo(new Wall);
        }
    }

    for (int i = 5; i <= 10; i++) {
        for (int j = 5; j <= 10; j++) {
            map->GetTile(i, j)->PlaceTo(new Floor);
        }
    }

    testMob = new Ghost;
	testMob_lastPosition = nullptr;
    map->GetTile(49, 49)->PlaceTo(testMob);
    test_dx = 1;
    test_dy = 0;

    for (int i = 85; i <= 95; i++) {
        for (int j = 85; j <= 95; j++) {
            Tile *tile = map->GetTile(i, j);
            tile->PlaceTo(new Floor);
        }
    }
}

void World::AddObject(Object *obj) {
    objects.push_back(uptr<Object>(obj));
}

Creature *World::CreateNewPlayerCreature() const {
    Creature *creature = new Human();
    map->GetTile(50, 50)->PlaceTo(creature);
    return creature;
}