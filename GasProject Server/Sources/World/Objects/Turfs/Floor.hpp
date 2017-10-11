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

    Floor(const Floor &object) = default;
    ~Floor() = default;
};