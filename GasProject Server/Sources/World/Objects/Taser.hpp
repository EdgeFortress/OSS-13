#pragma once

#include "Item.hpp"

class Taser : public Item {
protected:
    Taser();

public:

	virtual bool InteractedBy(Object *) final { return false; }

    void InteractWith(Object *) override;
};
