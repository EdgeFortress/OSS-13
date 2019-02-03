#include "Control.hpp"

#include <Server.hpp>
#include <World/World.hpp>
#include <World/Map.hpp>
#include <World/Objects/Creature.hpp>

#include <Shared/Math.hpp>

#include "Object.hpp"

Control::Control(float speed) : 
	speed(speed), 
    player(nullptr), 
    clickedObjectID(0)
{

}

void Control::Update(sf::Time timeElapsed) {
    ////
    //// Movement
    ////
	uf::vec2i order = moveOrder;
    moveOrder = {};

	// Form the intent based on the order
    if (order != uf::vec2i()) {
		owner->SetDirection(uf::VectToDirection(order));

		Tile *tile = owner->GetTile();
        if (tile) {
            uf::vec2i moveIntent;

            if (order.x) moveIntent.x = order.x;
            if (order.y) moveIntent.y = order.y;

            Tile *newTileDiag = tile->GetMap()->GetTile(tile->GetPos() + moveIntent);
            Tile *newTileX = tile->GetMap()->GetTile({ tile->GetPos().x + moveIntent.x, tile->GetPos().y });
            Tile *newTileY = tile->GetMap()->GetTile({ tile->GetPos().x, tile->GetPos().y + moveIntent.y });

            if (owner->GetDensity()) {
                if (!newTileDiag || newTileDiag->IsDense()) moveIntent = owner->GetMoveIntent();
                if (!newTileX || newTileX->IsDense()) moveIntent.x = 0;
                if (!newTileY || newTileY->IsDense()) moveIntent.y = 0;
            }

            owner->SetMoveIntent(moveIntent);
        }
	}

    ////
    //// Click
    ////
    if (clickedObjectID) {
        Object *clickedObject = CurThreadGame->GetWorld()->GetObject(clickedObjectID);

        if (auto *creature = dynamic_cast<Creature *>(owner))
            creature->TryInteractWith(clickedObject);

        clickedObjectID = 0;
    }
}

void Control::MoveCommand(uf::vec2i order) {
	moveOrder = order;
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

float Control::GetSpeed() const { return speed; }
Player *Control::GetPlayer() const { return player; }