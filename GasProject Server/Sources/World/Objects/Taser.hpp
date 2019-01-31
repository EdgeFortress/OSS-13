#pragma once

#include "Item.hpp"

class Taser : public Item {
public:
    Taser();

    void InteractWith(Object *) override;
};
