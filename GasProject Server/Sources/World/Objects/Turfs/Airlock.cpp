#include "Airlock.hpp"

#include <functional>

#include "Server.hpp"
#include "World/World.hpp"
#include "Network/Differences.hpp"

Airlock::Airlock() {
    name = "airlock";
    sprite = "airlock";
    density = true;
    opened = false;
    locked = false;
}

void Airlock::Update(sf::Time timeElapsed) {
	Turf::Update(timeElapsed);
	closeTimer.Update(timeElapsed);
}

bool Airlock::InteractedBy(Object *object) {
	if (!Turf::InteractedBy(object))
		return false;
    Activate();
	return true;
}

void Airlock::Activate() {
	// Blink if locked 
    if (locked) {
        if (!opened) {
            PlayAnimation("airlock_closed_animation");
        }
        return;
    }

	if (opened) {
		if (!PlayAnimation("airlock_closing"))
			return;
		SetSprite("airlock");
		opened = false;
		density = true;
	} else {
		if (!PlayAnimation("airlock_opening", std::bind(&Airlock::animationOpeningCallback, this)))
			return;
		SetSprite("airlock_opened");
		closeTimer.Start(AUTOCLOSE_TIME, std::bind(&Airlock::autocloseCallback, this));
	}
}

void Airlock::Lock() {
    locked = true;
}

void Airlock::Unlock() {
    locked = false;
}

void Airlock::animationOpeningCallback() {
	opened = true;
	density = false;
}

void Airlock::autocloseCallback() {
	if (opened)
		Activate();
}
