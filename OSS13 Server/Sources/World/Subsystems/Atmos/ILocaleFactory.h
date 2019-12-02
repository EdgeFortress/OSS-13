#pragma once

#include <memory>

#include <Algorithms/Graph.h>
#include <World/Subsystems/Atmos/IAtmosTile.h>
#include <World/Subsystems/Atmos/ILocale.h>

namespace subsystem::atmos {

struct ILocaleFactory {
	virtual ILocale *CreateLocale(IAtmosTile *tile) = 0;
	virtual ILocale *CreateLocale(std::unique_ptr<Graph<IAtmosTile *>> &&graph) = 0;
};

} // namespace subsystem::atmos
