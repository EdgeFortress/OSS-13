#include "Projectile.hpp"

#include "World/Tile.hpp"
#include "World/Objects.hpp"

Projectile::Projectile(uf::vec2i direction) :
	startTile(nullptr)
{
    name = "Stun Orb";
    sprite = "stunorb";
    density = false;
    layer = 100;
    SetConstSpeed(direction.normalize() * speed);
}

void Projectile::Update(std::chrono::microseconds timeElapsed) {
    Object::Update(timeElapsed);

	if (CheckIfJustCreated())
		startTile = GetTile();

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
