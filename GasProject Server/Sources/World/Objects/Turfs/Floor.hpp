#pragma once

#include "Turf.hpp"

class Floor : public Turf {
public:
    Floor() {
        layer = 15;
        sprite = "floor";
        name = "Floor";
        density = false;
    }

	virtual bool InteractedBy(Object *) final { return false; }
};