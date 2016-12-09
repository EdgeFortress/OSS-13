#pragma once

#include "Object.hpp"

class Item : public Object {
public:
    virtual void Update(sf::Time timeElapsed) { };
    virtual void Interact(Object *) { };
};