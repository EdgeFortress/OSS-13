#pragma once

#include "Object.hpp"

class Turf : public Object {
public:
    Turf() {}

    virtual void Update(sf::Time timeElapsed) { };
    virtual void Interact(Object *) { };
};

class Wall : public Turf {
public:
    Wall() {
        sprite = Global::Sprite::Wall;
        name = "Wall";
        density = true;
    }

    Wall(const Wall &object) = default;
    ~Wall() = default;
};

class Floor : public Turf {
public:
    Floor() {
        sprite = Global::Sprite::Floor;
        name = "Floor";
        density = false;
    }

    Floor(const Floor &object) = default;
    ~Floor() = default;
};

class Airlock : public Turf {
public:
    explicit Airlock() {
        sprite = Global::Sprite::Airlock;
        density = true;
    }

    Airlock(const Airlock &object) = default;
    ~Airlock() = default;
};