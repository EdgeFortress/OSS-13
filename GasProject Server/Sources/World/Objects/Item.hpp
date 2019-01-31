#pragma once

#include "Object.hpp"

class Item : public Object {
public:
    Item() {
        layer = 50;
        density = false;
    }

    void Update(sf::Time timeElapsed) override { };
    void InteractedBy(Object *) override { };

    virtual void InteractWith(Object *) = 0;
};