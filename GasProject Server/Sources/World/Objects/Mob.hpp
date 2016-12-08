#pragma once

#include "Object.hpp"

class Mob : public Object {
private:
    int moveY, moveX;
public:
    explicit Mob() {
        sprite = Global::Sprite::Mob;
        name = "Mob";
        density = false;
        moveY = 0; moveX = 0;
    }

    void MoveNorth() { moveY = -1; }
    void MoveSouth() { moveY = 1; }
    void MoveEast() { moveX = 1; }
    void MoveWest() { moveX = -1; }

    virtual void Update() override;
    virtual void Interact(Object *) { };
};