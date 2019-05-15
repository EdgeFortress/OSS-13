#pragma once

#include "Object.hpp"

class Projectile : public Object {
protected:
    Projectile(rpos direction);

public:
    // Object
	virtual void AfterCreation() final;
    virtual void Update(sf::Time timeElapsed) final;
	virtual bool InteractedBy(Object *) final { return false; }

protected:
    virtual void onHit(Object *);

private:
	const float speed = 10;

	Tile *startTile;
};
