#include "Taser.hpp"

#include <IGame.h>
#include <World/World.hpp>
#include <World/Tile.hpp>
#include <World/Objects/Projectile.hpp>

Taser::Taser() {
    sprite = "taser";
    name = "Taser";
}

void Taser::InteractWith(Object *obj) {
	if (!obj) 
		return;

	if (!obj->GetTile() || !GetTile())
		return;

	uf::vec2i direction = (obj->GetTile()->GetPos() - GetTile()->GetPos()).xy();
	GGame->GetWorld()->CreateObject<Projectile>(GetTile(), direction);
}
