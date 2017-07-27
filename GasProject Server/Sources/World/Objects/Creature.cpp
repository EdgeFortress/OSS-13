#include "Creature.hpp"
#include "Control.hpp"

Creature::Creature() {
	layer = 75;
	name = "Creature";
	float speed = 4;
	AddComponent(new Control(speed));
}

uint Creature::GetInvisibleVisibility() { return seeInvisibleAbility; }