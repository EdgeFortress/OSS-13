#pragma once

#include "World/Objects/Object.hpp"

class Turf : public Object {
protected:
    Turf() {
        layer = 25;
        movable = false;
    }

public:
};
