#include "Control.hpp"

#include "Object.hpp"
#include "World/World.hpp"

#include "Shared/Math.hpp"

#include "Server.hpp"

Control::Control(float speed) : 
	speed(speed), player(nullptr), clickedObjectID(0)
{

}

void Control::Update(sf::Time timeElapsed) {
    ////
    //// Movement
    ////
	const uf::vec2i order = moveOrder;
    moveOrder = {};

	// Form the intent based on the order
    if (order != uf::vec2i()) {
		owner->SetDirection(uf::VectToDirection(order));

		Tile *tile = owner->GetTile();

        auto lastMoveIntent = moveIntent;
        if (order.x) moveIntent.x = order.x;
        if (order.y) moveIntent.y = order.y;

		Tile *newTileDiag = tile->GetMap()->GetTile(tile->X() + moveIntent.x, tile->Y() + moveIntent.y);
		Tile *newTileX = tile->GetMap()->GetTile(tile->X() + moveIntent.x, tile->Y());
		Tile *newTileY = tile->GetMap()->GetTile(tile->X(), tile->Y() + moveIntent.y);

		if (owner->GetDensity()) {
            if (!newTileDiag || newTileDiag->IsDense()) moveIntent = lastMoveIntent;
			if (!newTileX || newTileX->IsDense()) moveIntent.x = 0;
			if (!newTileY || newTileY->IsDense()) moveIntent.y = 0;
		}
	}

    //// Aligning by tile boundaries if no intention to moving
    uf::vec2f shift = owner->GetShift();
    if (shift) {
        float delta = timeElapsed.asSeconds() * speed;
        // X
        if (!moveIntent.x && shift.x) {
            if (shift.x * uf::sgn(shift.x) > delta)
                shift.x -= uf::sgn(shift.x) * delta;
            else shift.x = 0;
        }
        // Y
        if (!moveIntent.y && shift.y) {
            if (shift.y * uf::sgn(shift.y) > delta)
                shift.y -= uf::sgn(shift.y) * delta;
            else shift.y = 0;
        }
        owner->SetShift(shift);
    }

	// If there is intention to move
    if (moveIntent) {
        owner->AddShift(moveIntent * speed * timeElapsed.asSeconds());
        uf::vec2f shift = owner->GetShift();

        if (shift) {
            int dx, dy;
            dy = dx = 0;
            uf::vec2f shiftChange;
            if (abs(shift.x) >= 0.5f) {
                dx += int(uf::sgn(shift.x) * floor(abs(shift.x) - 0.5f + 1.f));
                shiftChange.x -= dx;
            }
            if (abs(shift.y) >= 0.5f) {
                dy += int(uf::sgn(shift.y) * floor(abs(shift.y) - 0.5f + 1.f));
                shiftChange.y -= dy;
            }
            owner->AddShift(shiftChange);

            if (dx || dy) {
                Tile *tile = owner->GetTile();
                Tile *dest_tile = tile->GetMap()->GetTile(tile->X() + dx, tile->Y() + dy);
                if (dest_tile) {
                    dest_tile->MoveTo(owner);
                }

                if (dx) moveIntent.x = 0;
                if (dy) moveIntent.y = 0;
            }
        }
    }

    ////
    //// Click
    ////
    if (clickedObjectID) {
        Object *clickedObject = CurThreadGame->GetWorld()->GetObject(clickedObjectID);
        clickedObject->Interact(nullptr);
        clickedObjectID = 0;
    }
}

void Control::MoveCommand(uf::vec2i order) {
	moveOrder = order;
}

void Control::ClickObjectCommand(uint id) {
    clickedObjectID = id;
}

float Control::GetSpeed() const { return speed; }
Player *Control::GetPlayer() const { return player; }