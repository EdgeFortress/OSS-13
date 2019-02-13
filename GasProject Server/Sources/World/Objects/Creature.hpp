#pragma once

#include <SFML/System/Time.hpp>

#include "Object.hpp"

class Clothing;

class Creature : public Object {
protected:
	Creature();

public:
	virtual void Update(sf::Time timeElapsed) override;
    bool InteractedBy(Object *) override;

	virtual void Move(uf::vec2i order);
    virtual bool TryInteractWith(Object *);
    virtual void Stun();
	virtual bool PutOn(Clothing *);
	virtual void Drop() { };

	uint GetInvisibleVisibility() const;

	bool IsStunned() const;

protected:
	uint seeInvisibleAbility;

private:
	sf::Time stun;
};
