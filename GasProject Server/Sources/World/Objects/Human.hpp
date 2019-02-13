#pragma once

#include <unordered_map>

#include "Creature.hpp"

class Item;
class Clothing;

enum class ClothSlot : char;
typedef std::unordered_map<ClothSlot, Clothing *> ClothSlots;

class Human : public Creature {
protected:
	Human();
public:
	//virtual void Update(sf::Time timeElapsed) override;

	virtual bool RemoveObject(Object *) override;
	virtual bool InteractedBy(Object *) final;
    virtual bool TryInteractWith(Object *) override;
	virtual bool TakeItem(Item *);
	virtual bool PutOn(Clothing *) override;
	virtual void Drop() override;

	Item *GetSlotItem(ClothSlot) const;

protected:
	virtual void updateIcons() const override;

private:
	void pushToIcons(ClothSlot) const; // TODO: detail

private:
	std::vector<Item *> hands;
	ClothSlots clothes;
};
