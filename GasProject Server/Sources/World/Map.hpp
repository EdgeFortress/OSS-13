#pragma once

#include <vector>

#include "Shared/Types.hpp"
#include "Block.hpp"

using std::vector;
using namespace uf;

class Map {
public:
    explicit Map(const uint sizeX, const uint sizeY);

    //void GenerateLocals();

    //void NewLocal(Tile *tile);
    //void RemoveLocal(const Local *local);

    void ClearDiffs();
    void Update();

    vec2u GetSize() const;
    int GetNumOfBlocksX() const;
    int GetNumOfBlocksY() const;
    Tile *GetTile(int x, int y) const;
    Block *GetBlock(int x, int y) const;
    const vector< vector< uptr<Tile>> > &GetTiles() const;

private:
    vec2u size;
    vec2u numOfBlocks;

    vector< vector<uptr<Tile>> > tiles;
    vector< vector<uptr<Block>> > blocks;
    //list<uptr<Local>> locals;
};