#include "Ghost.hpp"

#include "Control.hpp"

#include <World/Map.hpp>
#include <World/Tile.hpp>

Ghost::Ghost() : hostControl(nullptr)
{
	layer = 80;
	sprite = "ghost";
	name = "Ghost";
	density = false;
	invisibility = 1;
	seeInvisibleAbility = 1;
}

void Ghost::MoveZ(int order) {
	if (!order) {
		return;
	}
	Tile *tile = GetTile();
	Tile *dest_tile = tile->GetMap()->GetTile(tile->GetPos() + rpos(0,0,order));
	if (dest_tile) {
		dest_tile->MoveTo(this);
	}
}

void Ghost::SetHostControl(Control *control) {
	hostControl = control;
}

Control *Ghost::GetHostControl() const { return hostControl; }