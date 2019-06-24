#pragma once

#include "Object.hpp"

class Clothing;

class Creature : public Object {
protected:
	Creature();

public:
	virtual void Update(std::chrono::microseconds timeElapsed) override;
    bool InteractedBy(Object *) override;

    virtual bool TryInteractWith(Object *);
    virtual void Stun();
	virtual bool PutOn(Clothing *);
	virtual void Drop() { };

	uint GetInvisibleVisibility() const;

	bool IsStunned() const;

protected:
	uint seeInvisibleAbility;

private:
	std::chrono::microseconds stun;
};
