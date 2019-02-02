#include "Human.hpp"

#include <World/Tile.hpp>
#include <World/Objects.hpp>

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

bool Human::PutOn(Item *item) {
	if (auto *clothing = dynamic_cast<Clothing *>(item)) {
		if (clothing->GetSlot() != ClothSlot::NONE)
			return false;

		if (clothes.find(clothing->GetSlot()) != clothes.end())
			return false;

		AddObject(clothing);
		clothes[clothing->GetSlot()] = clothing;
		clothing->SetSpriteState(Global::ItemSpriteState::ON_MOB);
	}
	return false;
}
