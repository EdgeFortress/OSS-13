#pragma once

#include "Item.hpp"

class Taser : public Item {
public:
    Taser();

	virtual bool InteractedBy(Object *) final { return false; }

    void InteractWith(Object *) override;
};
