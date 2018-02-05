#include "Taser.hpp"

#include "World/World.hpp"

Taser::Taser() {
    sprite = "taser";
    name = "Taser";
}

void Taser::Interact(Object *) {
    GetTile()->PlaceTo(new Projectile());
}