#include "Ghost.hpp"

#include "Control.hpp"

Ghost::Ghost() {
	sprite = Global::Sprite::Ghost;
	name = "Ghost";
	density = false;
	invisibility = 1;
	seeInvisibleAbility = 1;
}

void Ghost::SetHostControl(Control *control) {
	hostControl = control;
}

Control *Ghost::GetHostControl() { return hostControl; }