#include "Taser.hpp"

#include "World/World.hpp"
#include "Server.hpp"

Taser::Taser() {
    sprite = "taser";
    name = "Taser";
}

void Taser::InteractWith(Object *obj) {
	if (!obj) 
		return;

	if (!obj->GetTile() || !GetTile())
		return;

	uf::vec2f direction = obj->GetTile()->GetPos() - GetTile()->GetPos();
	CurThreadGame->GetWorld()->CreateObject<Projectile>(GetTile(), direction);
}
