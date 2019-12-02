#pragma once

#include <Shared/IFaces/INonCopyable.h>

#include <World/ISubsystem.h>
#include <World/Objects/ObjectHolder.h>
#include <World/Objects/Object.hpp>

using std::vector;

class Map;
class Creature;

class World : public ObjectHolder, public INonCopyable {
public:
	friend Object;

	World();

	void Initialize();
	void Update(std::chrono::microseconds timeElapsed);

	Object *CreateNewPlayerCreature();

	Object *GetObject(uint id) const;
	Map *GetMap() const;

private:
	void generateWorld();
	void createTestItems();

private:
	std::vector<std::unique_ptr<Map>> maps;
	std::vector<std::unique_ptr<ISubsystem>> subsystems;

	Object *testMob{nullptr};
	Tile *testMob_lastPosition;
	int test_dx;
	int test_dy;
	int mobsVelocity;
};
