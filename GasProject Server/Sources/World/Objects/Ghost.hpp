#pragma once

#include "Creature.hpp"

class Control;

class Ghost : public Creature {
private:
	Control *hostControl;

public:
	Ghost();

	virtual bool InteractedBy(Object *) final { return false; }

	void SetHostControl(Control *control);
	Control *GetHostControl() const;
};