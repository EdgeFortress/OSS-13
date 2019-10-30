#pragma once

#include <set>
#include <array>

#include <World/Subsystems/IAtmos.h>
#include <World/Subsystems/IAtmosTile.h>

class World;
class Map;

namespace subsystem {
namespace atmos {

class Locale : public ILocale {
public:
	Locale(IAtmosTile *tile);

	void AddConnection(IAtmosTile *first, IAtmosTile *second);
	void RemoveTile(IAtmosTile *tile);

	bool IsEmpty() const;

private:
	void addTile(IAtmosTile *tile);
	void merge(Locale *locale);
	bool isConnected();

	class Connection {
	public:
		Connection(IAtmosTile *first, IAtmosTile *second) {
			if (first > second)
				std::swap(first, second);
			tiles = { first, second };
		}

		const std::array<IAtmosTile *, 2> &GetTiles() const {
			return tiles;
		}

		bool operator<(const Connection &other) const {
			if (tiles[0] < other.tiles[0])
				return true;
			if (tiles[0] == other.tiles[0] && tiles[1] < other.tiles[1])
				return true;
			return false;
		}

	private:
		std::array<IAtmosTile *, 2> tiles;
	};

	std::set<IAtmosTile *> tiles;
	std::set<Connection> connections;
};

class Atmos : public IAtmos {
public:
	Atmos(World *world);

	// ISubsystem
	void Update(std::chrono::microseconds timeElapsed) final;

private:
	void generateLocales();
	void generateLocalesForMap(Map *map);
	void checkLocaleForTile(IAtmosTile *tile);

	std::set<IAtmosTile *> synchronizeTiles();

private:
	World *world;
	std::vector<std::unique_ptr<Locale>> locales;
};

} // namespace atmos
} // namespace subsystem
