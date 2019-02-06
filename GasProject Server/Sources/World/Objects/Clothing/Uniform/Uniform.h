#pragma once

#include <World/Objects/Clothing/Clothing.h>

class Uniform : public Clothing
{
public:
	Uniform();

	// Object
	virtual bool InteractedBy(Object *) final;
	// Item
	virtual void InteractWith(Object *) final;
};
