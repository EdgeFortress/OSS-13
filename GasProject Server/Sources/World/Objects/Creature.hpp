#pragma once

#include "Object.hpp"

class Creature : public Object {
private:
	sf::Vector2i moveIntent;

public:
	Creature();

	//virtual void Update(sf::Time timeElapsed) override = 0;
	virtual void Interact(Object *) { };
};