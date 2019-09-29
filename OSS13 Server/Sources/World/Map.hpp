#pragma once

#include <vector>

#include "Shared/Types.hpp"
#include "Tile.hpp"
#include "Atmos/Atmos.hpp"
#include "Shared/Grid.hpp"

using std::vector;

class Map {
public:
    explicit Map(const uint sizeX, const uint sizeY, const uint sizeZ);

    void ClearDiffs();
    void Update(std::chrono::microseconds timeElapsed);

    apos GetSize() const;
    Atmos *GetAtmos() const;
    Tile *GetTile(uf::vec3i) const;

private:
    uptr<Atmos> atmos;

    uf::Grid<uptr<Tile>> tiles;
};
