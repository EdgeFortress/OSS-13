#include "Map.hpp"

#include <plog/Log.h>

#include "Tile.hpp"
#include "Atmos/Atmos.hpp"
#include "Shared/Global.hpp"

Map::Map(const uint sizeX, const uint sizeY, const uint sizeZ)
{
	tiles.SetSize(sizeX, sizeY, sizeZ);
	for (auto &&[cell, pos] : tiles) {
		cell = std::make_unique<Tile>(this, pos);
	}
	LOGI << "Map is created with size: " << sizeX << "x" << sizeY << "x" << sizeZ;

	atmos = std::make_unique<Atmos>(this);
}

void Map::ClearDiffs() {
	for (auto &tile : tiles.Items())
		tile->ClearDiffs();
	network::protocol::Diff::ResetDiffCounter();
}

void Map::Update(std::chrono::microseconds timeElapsed) {
    for (auto &tile : tiles.Items())
		tile->Update(timeElapsed);
    atmos->Update(timeElapsed);
}

apos Map::GetSize() const { return tiles.GetSize(); }
Atmos* Map::GetAtmos() const { return atmos.get(); }

Tile *Map::GetTile(vec3i pos) const {
    if (pos >= vec3i(0) && pos < GetSize())
        return tiles.At(pos).get();
    return nullptr;
}
