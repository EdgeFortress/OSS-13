#include "Airlock.hpp"

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

void Airlock::Interact(Object *object) {
    Turf::Interact(object);
    Activate();
}

void Airlock::Activate() {
    if (locked) {
        if (!opened) {
            PlayAnimation("airlock_closed_animation");
        }
        return;
    }

    opened = !opened;
    density = !density;
    if (opened) {
        SetSprite("airlock_opened");
        PlayAnimation("airlock_opening");
    } else {
        SetSprite("airlock");
        PlayAnimation("airlock_closing");
    }
}

void Airlock::Lock() {
    locked = true;
}

void Airlock::Unlock() {
    locked = false;
}