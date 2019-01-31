#pragma once

#include "Object.hpp"

class Creature : public Object {
protected:
	uint seeInvisibleAbility;

public:
	Creature();

	//virtual void Update(sf::Time timeElapsed) override = 0;
    void InteractedBy(Object *) override;

    virtual void TryInteractWith(Object *);
    void Stun();

	uint GetInvisibleVisibility() const;
};