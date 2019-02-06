#include "Clothing.h"

Clothing::Clothing()
	: slot(ClothSlot::NONE)
{ }

ClothSlot Clothing::GetSlot() {
	return slot;
}
