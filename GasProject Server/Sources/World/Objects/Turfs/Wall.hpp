#pragma once

#include "Turf.hpp"

class Wall : public Turf {
protected:
    Wall() {
        sprite = "wall";
        name = "Wall";
        density = true;
    }

public:
	virtual bool InteractedBy(Object *) final { return false; }
};