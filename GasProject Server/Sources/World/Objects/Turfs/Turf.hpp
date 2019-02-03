#pragma once

#include "World/Objects/Object.hpp"

class Turf : public Object {
public:
    Turf() {
        layer = 25;
        movable = false;
    }

    virtual void InteractedBy(Object *) override;
};
