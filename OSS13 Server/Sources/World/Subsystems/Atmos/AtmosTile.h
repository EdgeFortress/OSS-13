#pragma once

#include <World/Subsystems/IAtmosTile.h>

#include <Shared/Geometry/DirectionSet.h>

class Object;

namespace subsystem {
namespace atmos {

class AtmosTile : public IAtmosTile {
public:
	bool SynchronizeAtmos() final;

	ILocale *GetLocale() final;
	void SetLocale(ILocale *locale) final;

	uf::DirectionSetFractional GetAirtightness() const final;
	float GetAirtightnessTo(uf::Direction direction) const final;

protected:
	virtual void addObject(Object *);
	virtual bool removeObject(Object *);

	void recountAirtightness();

private:
	ILocale *locale{};

	uf::DirectionSetFractional airtightness;
	uf::DirectionSetFractional airtightnessChanged;
};

} // namespace atmos
} // namespace subsystem
