#pragma once

#include "Item.hpp"

class Taser : public Item {
public:
    Taser();

    void Interact(Object *) override;
};