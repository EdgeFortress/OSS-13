#include "Block.hpp"

#include "Map.hpp"
#include "Tile.hpp"
#include "Network/Differences.hpp"

Block::Block(Map *map, int blockX, int blockY) :
    map(map), id(blockY * map->GetNumOfBlocksX() + blockX),
    blockX(blockX), blockY(blockY),
    size(Global::BLOCK_SIZE),
    tiles(size, vector<Tile *>(size))
{
    int y = 0;
    for (vector<Tile *> &vect : tiles) {
        int x = 0;
        for (Tile *&tile : vect) {
            tile = map->GetTile(uf::vec2i( x + blockX * size, y + blockY * size));
            x++;
        }
        y++;
    }
}

void Block::AddDiff(Diff *diff) {
    differences.push_back(sptr<Diff>(diff));
}

void Block::ClearDiffs() {
    differences.clear();
}

const BlockInfo Block::GetBlockInfo(uint visibility) {
    std::list<TileInfo> tilesInfo;
    for (auto &vect : tiles)
        for (auto &tile : vect)
            if (tile) tilesInfo.push_back(tile->GetTileInfo(visibility));
            else {
                tilesInfo.push_back(TileInfo());
            }
            return std::move(BlockInfo(blockX, blockY, tilesInfo));
}