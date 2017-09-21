#pragma once

#include <vector>

#include "Shared/Types.hpp"
#include "Block.hpp"
#include "Atmos/Atmos.hpp"

using std::vector;
using namespace uf;

class Map {
public:
    explicit Map(const uint sizeX, const uint sizeY);

    void ClearDiffs();
    void Update(sf::Time timeElapsed);

    vec2u GetSize() const;
    int GetNumOfBlocksX() const;
    int GetNumOfBlocksY() const;
    Atmos *GetAtmos() const;
    Tile *GetTile(int x, int y) const;
    Block *GetBlock(int x, int y) const;
    const vector< vector< uptr<Tile>> > &GetTiles() const;

private:
    vec2u size;
    vec2u numOfBlocks;

    uptr<Atmos> atmos;

    vector< vector<uptr<Tile>> > tiles;
    vector< vector<uptr<Block>> > blocks;
};