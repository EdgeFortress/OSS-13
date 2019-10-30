#include "AtmosTile.h"

#include <World/Objects/Object.hpp>
#include <World/Map.hpp>

using namespace subsystem::atmos;

bool AtmosTile::SynchronizeAtmos() {
	if (airtightness != airtightnessChanged) {
		airtightness = airtightnessChanged;
		return true;
	}
	return false;
}

ILocale *AtmosTile::GetLocale() { 
	return locale;
}

void AtmosTile::SetLocale(ILocale *locale) {
	this->locale = locale;
}

uf::DirectionSetFractional AtmosTile::GetAirtightness() const {
	return airtightness;
}

float AtmosTile::GetAirtightnessTo(uf::Direction direction) const {
	if (!uf::IsPureDirection(direction))
		return 0;

	auto neighbour = GetMap()->GetTile(GetPos() + uf::DirectionToVect(direction));
	return GetAirtightness().GetCumulativeFraction({ direction, uf::Direction::CENTER }) * neighbour->GetOpacity().GetCumulativeFraction({ uf::InvertDirection(direction), uf::Direction::CENTER });
}

void AtmosTile::addObject(Object *object) {
	if (!object->GetAirtightness().IsDefault())
		recountAirtightness();
}

bool AtmosTile::removeObject(Object *object) {
	if (!object->GetAirtightness().IsDefault())
		recountAirtightness();
	return true;
}

void AtmosTile::recountAirtightness() {
	airtightnessChanged.Reset();
	for (auto &object : Content()) {
		airtightnessChanged += object->GetAirtightness();
	}
}
