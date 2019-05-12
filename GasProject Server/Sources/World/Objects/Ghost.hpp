#pragma once

#include "Creature.hpp"

class Control;

class Ghost : public Creature {
protected:
	Ghost();

public:
	virtual bool InteractedBy(Object *) final { return false; }

	virtual void MoveZ(int order);

	void SetHostControl(Control *control);
	Control *GetHostControl() const;

private:
	Control *hostControl;
};
