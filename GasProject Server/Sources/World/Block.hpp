#pragma once

#include <vector>
#include <list>

#include "Shared/Types.hpp"
#include "Tile.hpp"

using std::vector;
using std::list;

class Map;
struct Diff;
struct BlockInfo;

class Block {
public:
    explicit Block(Map *map, int blockX, int blockY);

    void AddDiff(Diff *diff);
    const list<sptr<Diff>> GetDifferences() const { return differences; }
    void ClearDiffs();

    Tile *GetTile(int x, int y) const { return tiles[y][x]; }
    int X() const { return blockX; }
    int Y() const { return blockY; }
    int ID() const { return id; }
    const BlockInfo GetBlockInfo(uint visibility);

private:
    Map *map;

    int id;
    int blockX, blockY;
    uint size;

    vector< vector<Tile *> > tiles;
    list<sptr<Diff>> differences;
};