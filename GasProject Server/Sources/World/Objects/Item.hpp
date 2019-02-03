#pragma once

#include "Object.hpp"

class Item : public Object {
public:
    Item() {
        layer = 50;
        density = false;
    }

    void InteractedBy(Object *) override { };

	virtual void InteractWith(Object *) { };
};