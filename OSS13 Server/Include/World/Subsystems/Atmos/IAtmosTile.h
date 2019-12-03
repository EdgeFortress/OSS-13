#pragma once

#include <World/ITile.h>
#include <World/Subsystems/Atmos/ILocale.h>

namespace subsystem {
namespace atmos {

struct IAtmosTile : public ITile {
	virtual bool SynchronizeAtmos() = 0;

	virtual ILocale *GetLocale() = 0;
	virtual void SetLocale(ILocale *locale) = 0;

	virtual uf::DirectionSetFractional GetAirtightness() const = 0;
	virtual float GetAirtightnessTo(uf::Direction direction) const = 0;
};

} // namespace atmos
} // namespace subsystem
