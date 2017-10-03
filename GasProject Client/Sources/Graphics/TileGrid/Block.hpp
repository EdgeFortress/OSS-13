#pragma once

#include <vector>
#include <SFML/System.hpp>

#include "Shared/Types.hpp"
#include "Tile.hpp"

class TileGrid;

namespace sf {
    class Packet;
}

class Block {
private:
	TileGrid *tileGrid;
	int relX, relY;
	//int id;

	std::vector< std::vector< uptr<Tile> > > tiles;

public:
	explicit Block(TileGrid *tileGrid);
	Block(const Block &) = delete;
	Block &operator=(const Block &) = delete;
	~Block() = default;

	void Update(sf::Time);

	int GetRelX() const;
	int GetRelY() const;
	Tile *GetTile(int x, int y) const;
	TileGrid *GetTileGrid() const;

	friend sf::Packet &operator>>(sf::Packet &packet, Block &block);
	friend TileGrid;
};