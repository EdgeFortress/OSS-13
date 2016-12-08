#include "Mob.hpp"

#include "World/World.hpp"

void Mob::Update() {
    if (moveX || moveY) {
        Tile *dest_tile = tile->GetMap()->GetTile(tile->X() + moveX, tile->Y() + moveY);
        if (dest_tile)
            dest_tile->MoveTo(this);
    }
    moveY = 0; moveX = 0;
}