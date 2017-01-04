#pragma once

#include "Object.hpp"

class Item : public Object {
public:
    Item() {
        layer = 50;
    }

    virtual void Update(sf::Time timeElapsed) { };
    virtual void Interact(Object *) { };
};