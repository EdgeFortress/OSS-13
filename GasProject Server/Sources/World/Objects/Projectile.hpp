#pragma once

#include "Object.hpp"

class Projectile : public Object {
public:
    Projectile(uf::vec2f direction);

    // Object
	virtual void AfterCreation() final;
    virtual void Update(sf::Time timeElapsed) final;

    void InteractedBy(Object *) final;

protected:
    virtual void onHit(Object *);

private:
	const float speed = 10;

	Tile *startTile;
};
