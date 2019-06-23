#include "Creature.hpp"

#include <plog/Log.h>

#include <Network/Differences.hpp>
#include <World/Map.hpp>
#include <World/Tile.hpp>
#include <World/Objects/Control.hpp>

Creature::Creature() {
	layer = 75;
	name = "Creature";
	seeInvisibleAbility = 0;
	AddComponent(new Control());
}

void Creature::Update(sf::Time timeElapsed) {
	Object::Update(timeElapsed);
	if (stun > timeElapsed)
		stun -= timeElapsed;
	else
		stun = sf::Time::Zero;
}

bool Creature::InteractedBy(Object *) {
	return true;
}

bool Creature::TryInteractWith(Object *obj) { 
	if (!obj || IsStunned())
		return false;
	return true;
}

void Creature::Stun() {
	GetTile()->AddDiff(new StunnedDiff(this, sf::seconds(3)));
	stun = sf::seconds(3);
	LOGI << "Creature stunned" << std::endl;
}

bool Creature::PutOn(Clothing *) {
	return false;
}

uint Creature::GetInvisibleVisibility() const { return seeInvisibleAbility; }

bool Creature::IsStunned() const { return stun > sf::Time::Zero; }
