#pragma once

#include "World/Objects/Object.hpp"

class Turf : public Object {
public:
    Turf() {
        layer = 25;
        movable = false;
    }

    void Update(sf::Time timeElapsed) override { };
    void Interact(Object *) override;
};
