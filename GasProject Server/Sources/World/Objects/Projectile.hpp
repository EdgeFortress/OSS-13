#pragma once

#include "Object.hpp"

class Projectile : public Object {
public:
    Projectile();

    void Interact(Object *) final;
};