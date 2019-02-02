#pragma once

#include <unordered_map>

#include "Creature.hpp"

class Item;

enum class ClothSlot : char;
typedef std::unordered_map<ClothSlot, Item *> ClothSlots;

class Human : public Creature {
public:
	Human();
	//virtual void Update(sf::Time timeElapsed) override;

    virtual void TryInteractWith(Object *) override;
	virtual bool PutOn(Item *) override;

private:
    Item *hand;
	ClothSlots clothes;
};
