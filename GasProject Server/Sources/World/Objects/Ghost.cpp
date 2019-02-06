#include "Ghost.hpp"

#include "Control.hpp"

Ghost::Ghost() : hostControl(nullptr)
{
	layer = 80;
	sprite = "ghost";
	name = "Ghost";
	density = false;
	invisibility = 1;
	seeInvisibleAbility = 1;
}

void Ghost::SetHostControl(Control *control) {
	hostControl = control;
}

Control *Ghost::GetHostControl() const { return hostControl; }