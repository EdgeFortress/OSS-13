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
        sprite = Global::Sprite::Mob;
        name = "Mob";
        density = false;
        speed = 4;
    }

    void Move(sf::Vector2i order) { 
        std::unique_lock<std::mutex> lock(orderLock);
        if (order.x) moveOrder.x = order.x;
        if (order.y) moveOrder.y = order.y;
    }

    virtual void Update(sf::Time timeElapsed) override;
    virtual void Interact(Object *) { };
};
