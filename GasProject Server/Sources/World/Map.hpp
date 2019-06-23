#pragma once

#include <vector>

#include "Shared/Types.hpp"
#include "Tile.hpp"
#include "Atmos/Atmos.hpp"

using std::vector;
using namespace uf;

class Map {
public:
    explicit Map(const uint sizeX, const uint sizeY, const uint sizeZ);

    void ClearDiffs();
    void Update(std::chrono::microseconds timeElapsed);

    apos GetSize() const;
    Atmos *GetAtmos() const;
    Tile *GetTile(apos pos) const;
    const vector<uptr<Tile>> &GetTiles() const;

private:
    apos size;

    uptr<Atmos> atmos;

    vector<uptr<Tile>> tiles;
	uint flat_index(const apos c) const;
};
