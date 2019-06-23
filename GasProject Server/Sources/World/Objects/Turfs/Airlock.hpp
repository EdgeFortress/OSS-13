#pragma once

#include <World/Objects/Turfs/Turf.hpp>

#include <Shared/Timer.h>

class Airlock : public Turf {
protected:
    Airlock();

public:
	virtual void Update(std::chrono::microseconds timeElapsed) final;
    virtual bool InteractedBy(Object *) final;

    void Activate();
    void Lock();
    void Unlock();

private:
	void animationOpeningCallback();
	void autocloseCallback();

private:
	const std::chrono::seconds AUTOCLOSE_TIME{5};

	uf::Timer closeTimer;
    bool opened;
    bool locked;
};
