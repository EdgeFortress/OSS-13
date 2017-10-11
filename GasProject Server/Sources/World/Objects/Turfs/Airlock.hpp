#pragma once

#include "Turf.hpp"

class Airlock : public Turf {
public:
    explicit Airlock() {
        sprite = "airlock";
        density = true;
    }

    Airlock(const Airlock &object) = default;
    ~Airlock() = default;
};
