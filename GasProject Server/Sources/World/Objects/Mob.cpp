#include "Mob.hpp"

#include "Server.hpp"
#include "World/World.hpp"
#include "Network/Differences.hpp"

void Mob::Update(sf::Time timeElapsed) {
    //std::unique_lock<std::mutex> lock(orderLock);

	// Check for new move order
	sf::Vector2i order = getMoveOrder();
	if (order != sf::Vector2i()) {
		setDirection(Global::VectToDirection(order));

		Tile *newTileDiag = tile->GetMap()->GetTile(tile->X() + order.x, tile->Y() + order.y);
		Tile *newTileX = tile->GetMap()->GetTile(tile->X() + order.x, tile->Y() + moveIntent.x);
		Tile *newTileY = tile->GetMap()->GetTile(tile->X() + moveIntent.y, tile->Y() + order.y);

		if (order.x) moveIntent.x = order.x;
		if (order.y) moveIntent.y = order.y;

		if (!newTileDiag || newTileDiag->IsDense()) moveIntent = sf::Vector2i();
		if (!newTileX || newTileX->IsDense()) moveIntent.x = 0;
		if (!newTileY || newTileY->IsDense()) moveIntent.y = 0;
		if (!moveIntent.x && !moveIntent.y) return;

		//tile->GetBlock()->AddDiff(new ShiftDiff(id, Global::VectToDirection(order), speed));
	}

	// If there is intention to move
    if (moveIntent.x || moveIntent.y) {
        shift += speed * sf::Vector2f(moveIntent) * timeElapsed.asSeconds();

        if (shift.x || shift.y) {
            int dx, dy;
            dx = dy = 0;
            if (abs(shift.x) >= 0.5f) {
                dx += int(sgn(shift.x) * floor(abs(shift.x) - 0.5f + 1.f));
                shift.x -= dx;
            }
            if (abs(shift.y) >= 0.5f) {
                dy += int(sgn(shift.y) * floor(abs(shift.y) - 0.5f + 1.f));
                shift.y -= dy;
            }

            if (dx || dy) {
                Tile *dest_tile = tile->GetMap()->GetTile(tile->X() + dx, tile->Y() + dy);
				if (dest_tile) {
					dest_tile->MoveTo(this);
				}

                if (dx) moveIntent.x = 0;
                if (dy) moveIntent.y = 0;
            }
        }

		// If there is move intention only for Y
		if (!moveIntent.x && shift.x) {
			float newShiftX = shift.x - sgn(shift.x) * speed * timeElapsed.asSeconds();
			shift.x = sgn(shift.x) * sgn(newShiftX) > 0 ? newShiftX : 0;
		}
		// If there is move intention only for X
		if (!moveIntent.y && shift.y) {
			float newShiftY = shift.y - sgn(shift.y) * speed * timeElapsed.asSeconds();
			shift.y = sgn(shift.y) * sgn(newShiftY) > 0 ? newShiftY : 0;
		}
	}
	else // No exist move intention
		Object::Update(timeElapsed);
}