#pragma once

#include <unordered_map>

#include "Creature.hpp"

class Item;

enum class ClothSlot : char;
typedef std::unordered_map<ClothSlot, Item *> ClothSlots;

class Human : public Creature {
protected:
	Human();
public:
	//virtual void Update(sf::Time timeElapsed) override;

	virtual bool InteractedBy(Object *) final { return false; }
    virtual bool TryInteractWith(Object *) override;
	virtual bool TakeItem(Item *);
	virtual bool PutOn(Item *) override;
	virtual void Drop() override;

	Item *GetSlotItem(ClothSlot) const;

protected:
	virtual bool removeObjectFromContent(Object *) override;
	virtual void updateIcons() const override;

private:
	void pushToIcons(ClothSlot) const; // TODO: detail

private:
	std::vector<Item *> hands;
	ClothSlots clothes;
};
