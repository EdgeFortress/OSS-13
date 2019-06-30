#pragma once

#include "Object.hpp"

class Projectile : public Object {
protected:
    Projectile(uf::vec2i direction);

public:
    // Object
    virtual void Update(std::chrono::microseconds timeElapsed) final;
	virtual bool InteractedBy(Object *) final { return false; }

protected:
    virtual void onHit(Object *);

private:
	const float speed = 10;

	Tile *startTile;
};
