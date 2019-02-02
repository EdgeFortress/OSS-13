#pragma once

#include <World/Objects/Item.hpp>

enum class ClothSlot : char {
	NONE = 0,
	HEAD,
	UNIFORM,
	SHOES
};

class Clothing : public Item
{
public:
	Clothing();

	ClothSlot GetSlot();

protected:
	ClothSlot slot;
};
