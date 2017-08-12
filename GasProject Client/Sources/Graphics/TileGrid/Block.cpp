#include "Block.hpp"

#include <SFML/System.hpp>

#include "TileGrid.hpp"
#include "Tile.hpp"

Block::Block(TileGrid *tileGrid) :
	tileGrid(tileGrid), relX(relX), relY(relY),
	tiles(tileGrid->GetBlockSize())
{
	int y = 0;
	for (auto &vect : tiles) {
		vect.resize(tileGrid->GetBlockSize());
		int x = 0;
		for (auto &tile : vect) {
			tile.reset(new Tile(this, x, y));
			x++;
		}
		y++;
	}
}

int Block::GetRelX() const { return relX; }
int Block::GetRelY() const { return relY; }

Tile* Block::GetTile(int x, int y) const {
	if (x >= 0 && x < tileGrid->GetBlockSize() && y >= 0 && y < tileGrid->GetBlockSize())
		return tiles[y][x].get();
	return nullptr;
}

TileGrid *Block::GetTileGrid() const { return tileGrid; }

void Block::Update(sf::Time timeElapsed) {
	for (auto &vect : tiles)
		for (auto &tile : vect)
			tile->Update(timeElapsed);
}