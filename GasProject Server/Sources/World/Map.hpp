#pragma once

#include <vector>

#include "Shared/Types.hpp"
#include "Block.hpp"

using std::vector;

class Map {
public:
    explicit Map(const uint sizeX, const uint sizeY);

    //void GenerateLocals();

    //void NewLocal(Tile *tile);
    //void RemoveLocal(const Local *local);

    void ClearDiffs();
    void Update();

    Tile *GetTile(int x, int y) const;
    Block *GetBlock(int x, int y) const;
    int GetNumOfBlocksX() const;
    int GetNumOfBlocksY() const;

private:
    uint sizeX, sizeY;
    uint numOfBlocksX, numOfBlocksY;

    vector< vector<uptr<Tile>> > tiles;
    vector< vector<uptr<Block>> > blocks;
    //list<uptr<Local>> locals;
};