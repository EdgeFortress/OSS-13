#include "Map.hpp"

#include <plog/Log.h>

#include "Server.hpp"
#include "Tile.hpp"
#include "Atmos/Atmos.hpp"
#include "Shared/Global.hpp"
#include "Shared/Array.hpp"

Map::Map(const uint sizeX, const uint sizeY, const uint sizeZ) :
	size(sizeX, sizeY, sizeZ)
{
	tiles.reserve(sizeX*sizeY*sizeZ);
	for (uint z = 0; z < sizeZ; z++) {
		for (uint y = 0; y < sizeY; y++) {
			for (uint x = 0; x < sizeX; x++) {
				tiles.push_back(std::make_unique<Tile>(this, apos(x,y,z)));
			}
		}
	}
	LOGI << "Map is created with size: " << sizeX << "x" << sizeY << "x" << sizeZ << std::endl;

	atmos = std::make_unique<Atmos>(this);
}

void Map::ClearDiffs() {
    for (auto &tile : tiles)
        tile->ClearDiffs();
}

void Map::Update(sf::Time timeElapsed) {
    for (auto &tile : tiles)
		tile->Update(timeElapsed);
    atmos->Update(timeElapsed);
}

apos Map::GetSize() const { return size; }
Atmos* Map::GetAtmos() const { return atmos.get(); };

Tile *Map::GetTile(apos pos) const {
    if (pos < size)
        return tiles[flat_index(pos)].get();
    return nullptr;
}

uint Map::flat_index(const apos c) const {
	return uf::flat_index(c, size.x, size.y);
}

const vector< uptr<Tile>>& Map::GetTiles() const { return tiles; }