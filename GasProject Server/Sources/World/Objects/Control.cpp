#include "Control.hpp"

#include <IGame.h>
#include <World/World.hpp>
#include <World/Map.hpp>

#include <Shared/Math.hpp>

#include "Object.hpp"

Control::Control() : 
	Component("Control"),
	speed(4), 
    player(nullptr), 
    clickedObjectID(0)
{

}

void Control::Update(std::chrono::microseconds timeElapsed) 
{ }

void Control::MoveCommand(uf::vec2i order) {
	moveOrder = order;
}

void Control::MoveZCommand(bool order) {
	moveZOrder = order?1:-1;
}

void Control::ClickObjectCommand(uint id) {
    clickedObjectID = id;
}

void Control::SetOwner(Object *owner) {
    Component::SetOwner(owner);
    if (this->owner) {
        this->owner->SetMoveSpeed(speed);
    }
}

uint Control::GetSeeInvisibleAbility() const { return camera_seeInvisibleAbility; }
void Control::SetSeeInvisibleAbility(uint flags) { camera_seeInvisibleAbility = flags; };

float Control::GetSpeed() const { return speed; }
Player *Control::GetPlayer() const { return player; }

uf::vec2i Control::GetAndDropMoveOrder() { auto tmp = moveOrder; moveOrder = {}; return tmp; };
int Control::GetAndDropMoveZOrder() { auto tmp = moveZOrder; moveZOrder = {}; return tmp; };

Object *Control::GetAndDropClickedObject() {
	Object *obj = nullptr;
	if (clickedObjectID) {
		obj = GGame->GetWorld()->GetObject(clickedObjectID);
		clickedObjectID = 0;
	}
	return obj;
}
