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

bool Human::InteractedBy(Object *obj) {
	if (auto *human = dynamic_cast<Human *>(obj)) {
		for (auto &cloth: clothes) {
			if (human->TakeItem(cloth.second)) {
				return true;
			}
		}
		return false;
	}

	if (auto *cloth = dynamic_cast<Clothing *>(obj)) {
	    if (PutOn(cloth))
	        return true;
	    return false;
	}

	return false;
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

bool Human::PutOn(Clothing *clothing) {
	if (clothing->GetSlot() == ClothSlot::NONE)
		return false;
	
	auto iter = clothes.find(clothing->GetSlot());
	if (iter != clothes.end() && iter->second != nullptr)
		return false;

	AddObject(clothing);
	clothes[clothing->GetSlot()] = clothing;
	clothing->SetSpriteState(Global::ItemSpriteState::ON_MOB);

	return true;
}

void Human::Drop() { 
	if (!hands[0]) return;
	GetTile()->PlaceTo(hands[0]);
	askToUpdateIcons();
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

bool Human::RemoveObject(Object *objToRemove) {
	for (auto *&obj : hands) {
		if (obj == objToRemove) {
			obj = nullptr;
			break;
		}
	}

	if (auto *cloth = dynamic_cast<Clothing *>(objToRemove)) {
	    Clothing *&onMob = clothes[cloth->GetSlot()];
	    if (onMob == cloth)
	        onMob = nullptr;
	}

	return Object::RemoveObject(objToRemove);
}

void Human::updateIcons() const {
	Object::updateIcons();
	
	pushToIcons(ClothSlot::UNIFORM);
	pushToIcons(ClothSlot::LHAND);
	pushToIcons(ClothSlot::RHAND);
}
