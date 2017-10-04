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
public:
	explicit Block(TileGrid *tileGrid);
	Block(const Block &) = delete;
	Block &operator=(const Block &) = delete;
	~Block() = default;

	void Update(sf::Time);

	uf::vec2i GetRelPos() const;
	Tile *GetTile(int x, int y) const;
	TileGrid *GetTileGrid() const;

	friend sf::Packet &operator>>(sf::Packet &packet, Block &block);
	friend TileGrid;

private:
    TileGrid *tileGrid;
    uf::vec2i relPos;
    //int id;

    std::vector< std::vector< uptr<Tile> > > tiles;
};