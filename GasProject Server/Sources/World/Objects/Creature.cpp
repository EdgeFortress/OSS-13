#include "Creature.hpp"

#include "Server.hpp"
#include "Control.hpp"

Creature::Creature() {
	layer = 75;
	name = "Creature";
	const float speed = 4;
	AddComponent(new Control(speed));
}

void Creature::Interact(Object *) {
    Server::log << "Creature clicked" << std::endl;
}

uint Creature::GetInvisibleVisibility() const { return seeInvisibleAbility; }