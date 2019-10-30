#pragma once

#include <memory>
#include <vector>

#include <Shared/Types.hpp>
#include <Shared/Grid.hpp>
#include <Shared/IFaces/INonCopyable.h>

#include "Tile.hpp"

class Map : public INonCopyable {
public:
    explicit Map(const uint sizeX, const uint sizeY, const uint sizeZ);

    void ClearDiffs();
    void Update(std::chrono::microseconds timeElapsed);

    apos GetSize() const;
    Tile *GetTile(uf::vec3i) const;
	const uf::Grid<std::unique_ptr<Tile>> &GetTiles() const;

private:
    uf::Grid<std::unique_ptr<Tile>> tiles;
};
