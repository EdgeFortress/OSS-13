#include "Taser.hpp"

#include <Server.hpp>
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

	uf::vec2f direction = (obj->GetTile()->GetPos() - GetTile()->GetPos()).xy();
	CurThreadGame->GetWorld()->CreateObject<Projectile>(GetTile(), direction);
}
