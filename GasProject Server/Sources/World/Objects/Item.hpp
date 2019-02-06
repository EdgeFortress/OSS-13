#pragma once

#include "Object.hpp"

class Item : public Object {
public:
    Item() {
        layer = 50;
        density = false;
    }

	virtual void InteractWith(Object *) = 0;
};
