#pragma once

#include "Turf.hpp"

class Floor : public Turf {
protected:
    Floor() {
        layer = 15;
        sprite = "floor";
        name = "Floor";
        density = false;
    }

public:
	virtual bool InteractedBy(Object *) final { return false; }
};