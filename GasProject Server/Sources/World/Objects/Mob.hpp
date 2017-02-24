#pragma once

#include "Object.hpp"

class Mob : public Object {
private:
    sf::Vector2f shift;
    float speed; // speed (tiles/seconds)

    sf::Vector2i moveOrder;
    std::mutex orderLock;

public:
    explicit Mob() {
        layer = 75;
        sprite = Global::Sprite::Mob;
        name = "Mob";
        density = false;
        speed = 4;
    }

    void Move(sf::Vector2i order);

    virtual void Update(sf::Time timeElapsed) override;
    virtual void Interact(Object *) { };

    float GetSpeed() const { return speed; }
};
