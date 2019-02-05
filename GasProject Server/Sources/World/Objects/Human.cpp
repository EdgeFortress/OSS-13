#include "Human.hpp"

#include <Server.hpp>
#include <World/Tile.hpp>
#include <World/Objects.hpp>

#include <Shared/Global.hpp>

#include "Item.hpp"

Human::Human() :
	hands(2, nullptr)
{
    sprite = "human";
	name = "Human";
	density = true;
}

bool Human::TryInteractWith(Object *obj) {
	if (!Creature::TryInteractWith(obj))
		return false;

    if (hands[0]) {
        if (obj->InteractedBy(hands[0]))
			return true;

        if (auto *item = dynamic_cast<Item *>(hands[0]))
            item->InteractWith(obj);

        return true;
    }

	if (auto *item = dynamic_cast<Item *>(obj)) {
		if (TakeItem(item))
			return true;
	}

	obj->InteractedBy(this);
    return true;
}

bool Human::TakeItem(Item *item) {
	if (hands[0]) return false;
	if (!IsCloseTo(item)) return false;

	AddObject(item);
	hands[0] = item;
	item->SetSpriteState(Global::ItemSpriteState::DEFAULT);

	return true;
}

bool Human::PutOn(Item *item) {
	if (auto *clothing = dynamic_cast<Clothing *>(item)) {
		if (clothing->GetSlot() == ClothSlot::NONE)
			return false;

		if (clothes.find(clothing->GetSlot()) != clothes.end())
			return false;

		AddObject(clothing);
		clothes[clothing->GetSlot()] = clothing;
		clothing->SetSpriteState(Global::ItemSpriteState::ON_MOB);
	}
	return false;
}

Item *Human::GetSlotItem(ClothSlot slot) const {
	switch (slot) {
		case ClothSlot::LHAND:
			return hands[0];
		case ClothSlot::RHAND:
			return hands[1];
	}

	auto iter = clothes.find(slot);
	if (iter != clothes.end()) {
		return iter->second;
	}
	return nullptr;
}

void Human::pushToIcons(ClothSlot slot) const {
	auto *item = GetSlotItem(slot);

	auto state = Global::ItemSpriteState::ON_MOB;
	switch (slot) {
		case ClothSlot::LHAND:
			state = Global::ItemSpriteState::IN_HAND_LEFT;
			break;
		case ClothSlot::RHAND:
			state = Global::ItemSpriteState::IN_HAND_RIGHT;
			break;
	}

	if (item)
		icons.push_back(Server::Get()->RM->GetIconInfo(item->GetSprite(), state));
}

void Human::updateIcons() const {
	Object::updateIcons();
	
	pushToIcons(ClothSlot::UNIFORM);
	pushToIcons(ClothSlot::LHAND);
	pushToIcons(ClothSlot::RHAND);
}
