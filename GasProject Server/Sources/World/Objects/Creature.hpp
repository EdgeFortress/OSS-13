#pragma once

#include <SFML/System/Time.hpp>

#include "Object.hpp"

class Item;

class Creature : public Object {
public:
	Creature();

	virtual void Update(sf::Time timeElapsed) override;
    void InteractedBy(Object *) override;

	virtual void Move(uf::vec2i order);
    virtual bool TryInteractWith(Object *);
    virtual void Stun();
	virtual bool PutOn(Item *);

	uint GetInvisibleVisibility() const;

	bool IsStunned() const;

protected:
	uint seeInvisibleAbility;

private:
	sf::Time stun;
};
