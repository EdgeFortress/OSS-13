#pragma once

#include "Creature.hpp"

class Control;

class Ghost : public Creature {
private:
	Control *hostControl;

public:
	Ghost();

	void SetHostControl(Control *control);
	Control *GetHostControl() const;
};