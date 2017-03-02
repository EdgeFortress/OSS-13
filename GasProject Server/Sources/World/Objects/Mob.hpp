#pragma once

#include "Object.hpp"
#include "World/Controllable.hpp"

class Mob : public Controllable {
private:
    sf::Vector2i moveIntent;

public:
    explicit Mob() {
        layer = 75;
        sprite = Global::Sprite::Mob;
        name = "Mob";
        density = false;
        speed = 4;
    }

    virtual void Update(sf::Time timeElapsed) override;
    virtual void Interact(Object *) { };

    float GetSpeed() const { return speed; }
};
