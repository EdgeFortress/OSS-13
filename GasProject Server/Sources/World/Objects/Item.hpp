#pragma once

#include "Object.hpp"

class Item : public Object {
public:
    virtual void Update() { };
    virtual void Interact(Object *) { };
};