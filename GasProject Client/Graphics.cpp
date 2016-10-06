#include "TileGrid.hpp"

Block::Block(TileGrid *tileGrid) : tileGrid(tileGrid),
								   tiles(tileGrid->BlockSize,
								   vector<Tile *>(tileGrid->BlockSize)) {
	for (auto &vect : tiles)
		for (auto &tile : vect)
			tile = new Tile(this);
}