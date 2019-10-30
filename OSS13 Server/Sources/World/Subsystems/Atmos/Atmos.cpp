#include "Atmos.h"

#include <algorithm>

#include <World/World.hpp>
#include <World/Map.hpp>
#include <World/Tile.hpp>

using namespace subsystem;
using namespace atmos;

Locale::Locale(IAtmosTile *tile) {
	EXPECT(tile);
	tiles.insert(tile);
	tile->SetLocale(this);
}

void Locale::AddConnection(IAtmosTile *first, IAtmosTile *second) {
	bool hasAtLeastOneTile = false;
	for (auto &tile : {first, second}) {
		EXPECT(tile);
		if (tiles.find(tile) != tiles.end())
			hasAtLeastOneTile = true;
		else
			addTile(tile);
	}
	EXPECT(hasAtLeastOneTile);
	connections.insert(Connection(first, second));
}

void Locale::RemoveTile(IAtmosTile *tileToRemove) {
	EXPECT(tileToRemove->GetLocale() == this);

	tiles.erase(tileToRemove);

	size_t connectionsSize = connections.size();

	// Change in C++20:
	//
	//std::erase_if(connections.begin(), connections.end(), 
	//	[tileToRemove](const Locale::Connection &connection) {
	//		for (auto &tile : connection.GetTiles())
	//			if (tile == tileToRemove)
	//				return true;
	//		return false;
	//	}
	//);
	for (auto iter = connections.begin(); iter != connections.end(); ) {
		auto &connection = *iter;
		bool erase = false;
		for (auto &tile : connection.GetTiles()) {
			if (tile == tileToRemove) {
				erase = true;
				break;
			}
		}
		if (erase) {
			iter = connections.erase(iter);
		} else
			iter++;
	}
	////

	size_t removedConnectionsCount = connectionsSize - connections.size();
	if (removedConnectionsCount > 1) {
		if (!isConnected()) {
			// ... do something
		}
	}

	tileToRemove->SetLocale(nullptr);
}

void Locale::addTile(IAtmosTile *tile) {
	Locale *lastLocale = reinterpret_cast<Locale *>(tile->GetLocale());
	if (lastLocale) {
		merge(lastLocale);
	} else {
		tiles.insert(tile);
		tile->SetLocale(this);
	}
}

void Locale::merge(Locale *locale) {
	connections.insert(locale->connections.begin(), locale->connections.end());
	locale->connections.clear();

	tiles.insert(locale->tiles.begin(), locale->tiles.end());
	for (auto tile: locale->tiles)
		tile->SetLocale(this);
	locale->tiles.clear();
}

bool Locale::isConnected() {
	return true;
}

bool Locale::IsEmpty() const {
	return tiles.empty();
}

Atmos::Atmos(World *world) :
	world(world)
{
	EXPECT(world);
	generateLocales();
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
		locales.push_back(std::make_unique<Locale>(tile));
}

void Atmos::Update(std::chrono::microseconds /*timeElapsed*/) {
	auto updatedTiles = synchronizeTiles();

	for (auto &tile : updatedTiles) {
		if (tile->GetLocale())
			static_cast<Locale *>(tile->GetLocale())->RemoveTile(tile);
	}

	for (auto &tile : updatedTiles) {
		checkLocaleForTile(tile);
	}
}

std::set<IAtmosTile *> Atmos::synchronizeTiles() {
	std::set<IAtmosTile *> updatedTiles;

	for (auto &tile : world->GetMap()->GetTiles().Items()) {
		if (tile->SynchronizeAtmos())
			updatedTiles.insert(tile.get());
	}

	return updatedTiles;
}
