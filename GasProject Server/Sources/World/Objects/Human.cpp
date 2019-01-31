#include "Human.hpp"

#include "World/Tile.hpp"
#include "Item.hpp"

Human::Human() :
    hand(nullptr)
{
    sprite = "human";
	name = "Human";
	density = true;
}

void Human::TryInteractWith(Object *obj) {
    if (hand) {
        obj->InteractedBy(hand);

        if (auto *item = dynamic_cast<Item *>(hand))
            item->InteractWith(obj);

        return;
    }

    if (auto *item = dynamic_cast<Item *>(obj)) {
		AddObject(item);
        hand = item;
        return;
    }

    return;
}
