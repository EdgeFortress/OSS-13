#include "Creature.hpp"

#include "Server.hpp"
#include "Control.hpp"
#include "World/Tile.hpp"

Creature::Creature() {
	layer = 75;
	name = "Creature";
	const float speed = 4;
	AddComponent(new Control(speed));
}

void Creature::InteractedBy(Object *) {
    Server::log << "Creature clicked" << std::endl;
}

void Creature::TryInteractWith(Object *obj) { }

void Creature::Stun() {
    Server::log << "Creature stunned" << std::endl;
}

uint Creature::GetInvisibleVisibility() const { return seeInvisibleAbility; }