#include "Uniform.h"

Uniform::Uniform() {
	slot = ClothSlot::UNIFORM;
	sprite = "grey";
	name = "uniform";
}

bool Uniform::InteractedBy(Object *) { return false; }
void Uniform::InteractWith(Object *) { };
