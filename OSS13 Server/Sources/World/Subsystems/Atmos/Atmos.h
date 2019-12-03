#pragma once

#include <set>
#include <array>

#include <World/Subsystems/Atmos/IAtmos.h>
#include <World/Subsystems/Atmos/IAtmosTile.h>

#include "ILocaleFactory.h"
#include "Locale.h"

class World;
class Map;

namespace subsystem {
namespace atmos {

class Atmos : public IAtmos, public ILocaleFactory {
public:
	Atmos(World *world);

	// ISubsystem
	void Update(std::chrono::microseconds timeElapsed) final;

	// ILocaleFactory
	ILocale *CreateLocale(IAtmosTile *tile) final;
	ILocale *CreateLocale(std::unique_ptr<Graph<IAtmosTile *>> &&graph) final;

private:
	void generateLocales();
	void generateLocalesForMap(Map *map);

	void updateTiles();
	void updateLocales(std::chrono::microseconds timeElapsed);

	void checkLocaleForTile(IAtmosTile *tile);

	std::set<IAtmosTile *> synchronizeTiles();

private:
	World *world;
	std::vector<std::unique_ptr<Locale>> locales;
	std::vector<std::unique_ptr<Locale>> newLocales;
};

} // namespace atmos
} // namespace subsystem
