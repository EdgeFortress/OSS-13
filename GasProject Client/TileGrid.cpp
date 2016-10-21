#include "TileGrid.hpp"

void TileGrid::Draw(sf::RenderWindow *window)
{
	
}

Tile* Block::GetTile(int x, int y) {
	if (x >= 0 && x < tileGrid->BlockSize && y >= 0 && y < tileGrid->BlockSize) return tiles[y][x];
	cout << "Can't return tile " << x << ", " << y << endl;
	return nullptr;
}