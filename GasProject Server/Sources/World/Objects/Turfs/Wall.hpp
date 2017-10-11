#pragma once

#include "Turf.hpp"

class Wall : public Turf {
public:
    Wall() {
        sprite = "wall";
        name = "Wall";
        density = true;
    }

    Wall(const Wall &object) = default;
    ~Wall() = default;
};