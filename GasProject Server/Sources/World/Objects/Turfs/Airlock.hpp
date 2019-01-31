#pragma once

#include "Turf.hpp"

class Airlock : public Turf {
public:
    Airlock();

    Airlock(const Airlock &object) = default;
    ~Airlock() = default;

    void InteractedBy(Object *) override;

    void Activate();
    void Lock();
    void Unlock();

private:
    bool opened;
    bool locked;
};
