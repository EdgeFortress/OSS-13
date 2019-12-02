#include "Atmos.h"

#include <algorithm>

#include <World/World.hpp>
#include <World/Map.hpp>
#include <World/Tile.hpp>

using namespace subsystem;
using namespace atmos;

Atmos::Atmos(World *world) :
	world(world)
{
	EXPECT(world);
	generateLocales();
}

void Atmos::Update(std::chrono::microseconds timeElapsed) {
	updateTiles();
	updateLocales(timeElapsed);
}

void Atmos::updateTiles() {
	auto updatedTiles = synchronizeTiles();

	for (auto &tile : updatedTiles) {
		if (tile->GetLocale())
			static_cast<Locale *>(tile->GetLocale())->RemoveTile(tile);
	}

	for (auto &tile : updatedTiles) {
		checkLocaleForTile(tile);
	}
}

void Atmos::updateLocales(std::chrono::microseconds timeElapsed) {
	std::move(newLocales.begin(), newLocales.end(), std::back_inserter(locales));
	newLocales.clear();

	for (auto iter = locales.begin(); iter != locales.end();) {
		auto *locale = iter->get();
		locale->Update(timeElapsed);

		if (!locale->GetSize())
			iter = locales.erase(iter);
		else
			iter++;
	}
}

ILocale *Atmos::CreateLocale(IAtmosTile *tile) {
	EXPECT(tile);
	EXPECT(!tile->GetLocale());
	newLocales.push_back(std::make_unique<Locale>(this, tile));
	return newLocales.back().get();
}

ILocale *Atmos::CreateLocale(std::unique_ptr<Graph<IAtmosTile *>> &&graph) {
	EXPECT(graph->Count());
	newLocales.push_back(std::make_unique<Locale>(this, std::forward<std::unique_ptr<Graph<IAtmosTile *>>>(graph)));
	return newLocales.back().get();
}

void Atmos::generateLocales() {
	synchronizeTiles();
	generateLocalesForMap(world->GetMap());
}

void Atmos::generateLocalesForMap(Map *map) {
	EXPECT(map);
	for (auto &tile : map->GetTiles().Items()) {
		checkLocaleForTile(tile.get());
	}
}

void Atmos::checkLocaleForTile(IAtmosTile *tile) {
	EXPECT(tile);

	LOGD_(1) << "Check tile " << tile->GetPos().toString();

	if (tile->GetAirtightness().GetFraction(uf::Direction::CENTER) == 0.f)
		return;

	for (auto direction : uf::PURE_DIRECTIONS_LIST) {
		if (direction == uf::Direction::CENTER)
			continue;
		auto neighbour = reinterpret_cast<IAtmosTile *>(tile->StepTo(direction));
		if (neighbour && tile->GetAirtightnessTo(direction) == 1.f) {
			Locale *locale = reinterpret_cast<Locale *>(tile->GetLocale());
			if (!locale)
				locale = reinterpret_cast<Locale *>(neighbour->GetLocale());

			if (locale) {
				locale->AddConnection(tile, neighbour);
				continue;
			}
		}
	}

	if (!tile->GetLocale())
		CreateLocale(tile);
}

std::set<IAtmosTile *> Atmos::synchronizeTiles() {
	std::set<IAtmosTile *> updatedTiles;

	for (auto &tile : world->GetMap()->GetTiles().Items()) {
		if (tile->SynchronizeAtmos())
			updatedTiles.insert(tile.get());
	}

	return updatedTiles;
}
