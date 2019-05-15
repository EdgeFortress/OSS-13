#include "Projectile.hpp"

#include "World/Tile.hpp"
#include "World/Objects.hpp"

Projectile::Projectile(rpos direction) :
	startTile(nullptr)
{
    name = "Stun Orb";
    sprite = "stunorb";
    density = false;
    layer = 100;
    SetConstSpeed(uf::vec2f(direction.x, direction.y).normalize() * speed);
}

void Projectile::AfterCreation() {
	if (!GetTile())
		throw std::exception(); // "Unexpected: projectile's tile is null after creation!"
	startTile = GetTile();

	Object::AfterCreation();
}

void Projectile::Update(sf::Time timeElapsed) {
    Object::Update(timeElapsed);

	if (!GetTile())
		throw std::exception(); // "Unexpected: projectile's tile is null!"

    if (startTile != GetTile() && GetTile()->IsDense()) {
        onHit(GetTile()->GetDenseObject());
        Delete();
    }
}

void Projectile::onHit(Object *obj) {
    if (auto *creature = dynamic_cast<Creature *>(obj))
        creature->Stun();
}
