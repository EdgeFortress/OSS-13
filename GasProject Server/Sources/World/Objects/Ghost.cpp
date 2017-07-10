#include "Ghost.hpp"

Ghost::Ghost() {
	sprite = Global::Sprite::Ghost;
	name = "Ghost";
	density = false;
	invisibility = 1;
	seeInvisibleAbility = 1;
}