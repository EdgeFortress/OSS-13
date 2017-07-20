#include "Control.hpp"
#include "Object.hpp"
#include "World/World.hpp"

#include "Shared/Math.hpp"

//#include "Server.hpp"

Control::Control(float speed) : 
	speed(speed) {

}

void Control::Update(sf::Time timeElapsed) {
	//Server::log << "Control::Update" << endl;

	sf::Vector2i order = moveOrder;
	moveOrder = sf::Vector2i();

	// Form the intent based on the order
	if (order != sf::Vector2i()) {
		owner->SetDirection(uf::VectToDirection(order));

		Tile *tile = owner->GetTile();

		Tile *newTileDiag = tile->GetMap()->GetTile(tile->X() + order.x, tile->Y() + order.y);
		Tile *newTileX = tile->GetMap()->GetTile(tile->X() + order.x, tile->Y() + moveIntent.x);
		Tile *newTileY = tile->GetMap()->GetTile(tile->X() + moveIntent.y, tile->Y() + order.y);

		if (order.x) moveIntent.x = order.x;
		if (order.y) moveIntent.y = order.y;

		if (!newTileDiag || newTileDiag->IsDense()) moveIntent = sf::Vector2i();
		if (!newTileX || newTileX->IsDense()) moveIntent.x = 0;
		if (!newTileY || newTileY->IsDense()) moveIntent.y = 0;
		if (!moveIntent.x && !moveIntent.y) return;
	}

	// If there is intention to move
	if (moveIntent.x || moveIntent.y) {
		owner->AddShift(speed * sf::Vector2f(moveIntent) * timeElapsed.asSeconds());
		sf::Vector2f shift = owner->GetShift();

		if (shift.x || shift.y) {
			int dx, dy;
			sf::Vector2f shiftChange;
			dx = dy = 0;
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

		shift = owner->GetShift();

		// If there is move intention only for Y
		if (!moveIntent.x && shift.x) {
			float newShiftX = shift.x - uf::sgn(shift.x) * speed * timeElapsed.asSeconds();
			shift.x = uf::sgn(shift.x) * uf::sgn(newShiftX) > 0 ? newShiftX : 0;
		}
		// If there is move intention only for X
		if (!moveIntent.y && shift.y) {
			float newShiftY = shift.y - uf::sgn(shift.y) * speed * timeElapsed.asSeconds();
			shift.y = uf::sgn(shift.y) * uf::sgn(newShiftY) > 0 ? newShiftY : 0;
		}
		owner->SetShift(shift);
	}
	else // No exist move intention
	{
		if (speed) {
			sf::Vector2f shift = owner->GetShift();
			if (shift.x) {
				float newShiftX = shift.x - uf::sgn(shift.x) * speed * timeElapsed.asSeconds();
				shift.x = uf::sgn(shift.x) * uf::sgn(newShiftX) > 0 ? newShiftX : 0;
			}
			if (shift.y) {
				float newShiftY = shift.y - uf::sgn(shift.y) * speed * timeElapsed.asSeconds();
				shift.y = uf::sgn(shift.y) * uf::sgn(newShiftY) > 0 ? newShiftY : 0;
			}
			owner->SetShift(shift);
		}
	}
}

void Control::MoveCommand(sf::Vector2i order) {
	moveOrder = order;
}

float Control::GetSpeed() const { return speed; }