#pragma once

#include "Creature.hpp"

class Item;

class Human : public Creature {
public:
	Human();
	//virtual void Update(sf::Time timeElapsed) override;

    virtual void TryInteractWith(Object *) override;

private:
    Item *hand;
};
