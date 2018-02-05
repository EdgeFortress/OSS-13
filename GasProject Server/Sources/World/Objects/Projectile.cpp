#include "Projectile.hpp"

Projectile::Projectile() {
    name = "Stun Orb";
    sprite = "stunorb";
    density = false;
    layer = 100;
    SetConstSpeed({ 0, 10 });
}

void Projectile::Interact(Object *) { }

