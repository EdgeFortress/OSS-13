#pragma once

#include "Turf.hpp"

class Wall : public Turf {
public:
    Wall() {
        sprite = "wall";
        name = "Wall";
        density = true;
    }

	virtual bool InteractedBy(Object *) final { return false; }
};