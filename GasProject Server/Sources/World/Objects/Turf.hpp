#pragma once

#include "Object.hpp"

class Turf : public Object {
public:
    Turf() {
        layer = 25;
        movable = false;
    }

    void Update(sf::Time timeElapsed) override { };
    void Interact(Object *) override { };
};

class Wall : public Turf {
public:
    Wall() {
        sprite = "wall";
        name = "Wall";
        density = true;
    }

    Wall(const Wall &object) = default;
    ~Wall() = default;
};

class Floor : public Turf {
public:
    Floor() {
        layer = 15;
        sprite = "floor";
        name = "Floor";
        density = false;
    }

    Floor(const Floor &object) = default;
    ~Floor() = default;
};

class Airlock : public Turf {
public:
    explicit Airlock() {
        sprite = "door";
        density = true;
    }

    Airlock(const Airlock &object) = default;
    ~Airlock() = default;
};