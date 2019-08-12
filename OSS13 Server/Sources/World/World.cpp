#include "World.hpp"

#include "Map.hpp"
#include "Tile.hpp"
#include "Objects.hpp"
#include "Objects/Control.hpp"
#include "Player.hpp"

#include <Shared/ErrorHandling.h>

using namespace std::string_literals;

World::World() : 
	map(new Map(100, 100, 3))
{ }

void World::Update(std::chrono::microseconds timeElapsed) {
	map->ClearDiffs();

	// Simple walking mob AI for moving testing
	if (testMob) {
		if (testMob_lastPosition != testMob->GetTile()) {
			testMob_lastPosition = testMob->GetTile();
			const int x = testMob->GetTile()->GetPos().x + test_dx;
			const int y = testMob->GetTile()->GetPos().y + test_dy;
			if (test_dx == 1 && x == 53) test_dx = 0, test_dy = 1;
			if (test_dy == 1 && y == 53) test_dx = -1, test_dy = 0;
			if (test_dx == -1 && x == 47) test_dx = 0, test_dy = -1;
			if (test_dy == -1 && y == 47) test_dx = 1, test_dy = 0;

			dynamic_cast<Control *>(testMob->GetComponent("Control"))->MoveCommand(sf::Vector2i(test_dx, test_dy));
		}
	}
    
    map->Update(timeElapsed);

    // update objects
    for (uint i = 0; i < objects.size(); i++) {
        if (!objects[i]) continue; // already deleted
        if (objects[i]->CheckIfMarkedToBeDeleted()) {
			// Broke cycle ref: PyObject <-> C++ Object
			if (objects[i].use_count() > 2) { // Object can be captured by Tile's diffs list
				auto deleteAttempts = objects[i]->IncreaseDeleteAttempts();
				CHECK_WITH_MSG(deleteAttempts == 5, "Object (id="s + std::to_string(i) + ") wasn't deleted 5 times!"s);
				CHECK_WITH_MSG(deleteAttempts == 20, "Object (id="s + std::to_string(i) + ") can't be deleted!"s);
			} else {
				objects[i].reset();
				free_ids.push_back(i + 1);
			}
			continue;
        }
		if (objects[i]->CheckIfJustCreated()) // don't update objects created at current tick
			continue;
        objects[i]->Update(timeElapsed);
    }
}

void World::CreateTestItems() {
	CreateScriptObject("Objects.Items.Taser", {50, 51, 0});
	CreateScriptObject("Objects.Turfs.Window", {50, 52, 0});

	testMob = CreateScriptObject("Objects.Creatures.Ghost", { 49, 49, 0 });
	testMob_lastPosition = nullptr;

	test_dx = 1;
	test_dy = 0;
}

Object *World::CreateNewPlayerCreature() {
	auto human = CreateScriptObject("Objects.Creatures.Human", { 50, 50, 0 });

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
