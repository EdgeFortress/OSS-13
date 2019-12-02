#pragma once

#include <World/Subsystems/Atmos/IAtmosTile.h>
#include <World/Subsystems/Atmos/ILocale.h>

#include <Shared/Geometry/DirectionSet.h>

class Object;

namespace subsystem {
namespace atmos {

class AtmosTile : public IAtmosTile {
public:
	void RecountAirtightness();
	bool SynchronizeAtmos() final;

	ILocale *GetLocale() final;
	void SetLocale(ILocale *locale) final;

	uf::DirectionSetFractional GetAirtightness() const final;
	float GetAirtightnessTo(uf::Direction direction) const final;

protected:
	void addObject(Object *);
	void removeObject(Object *);

	void recountAirtightness();

private:
	ILocale *locale{};

	bool needToRecoundAirtightness{false};
	uf::DirectionSetFractional airtightness;
	uf::DirectionSetFractional airtightnessChanged;
};

} // namespace atmos
} // namespace subsystem
