#include "Map.hpp"

#include "Server.hpp"
#include "Tile.hpp"
#include "Block.hpp"
#include "Shared/Global.hpp"

Map::Map(const uint sizeX, const uint sizeY) :
    sizeX(sizeX), sizeY(sizeY),
    numOfBlocksX(sizeX / Global::BLOCK_SIZE + (sizeX % Global::BLOCK_SIZE ? 1 : 0)),
    numOfBlocksY(sizeY / Global::BLOCK_SIZE + (sizeY % Global::BLOCK_SIZE ? 1 : 0)),
    tiles(sizeY),
    blocks(numOfBlocksY)
{
    for (vector<uptr<Tile>> &vect : tiles) {
        vect = vector<uptr<Tile>>(sizeX);
    }

    int y = 0;
    for (vector<uptr<Tile>> &vect : tiles) {
        int x = 0;
        for (uptr<Tile> &tile : vect) {
            tile.reset(new Tile(this, x, y));
            x++;
        }
        y++;
    }

    for (vector<uptr<Block>> &vect : blocks) {
        vect = vector<uptr<Block>>(numOfBlocksY);
    }

    y = 0;
    for (vector<uptr<Block>> &vect : blocks) {
        int x = 0;
        for (uptr<Block> &tile : vect) {
            tile.reset(new Block(this, x, y));
            x++;
        }
        y++;
    }

    Server::log << "Map created: " << sizeX << "x" << sizeY << " (" << numOfBlocksX << "x" << numOfBlocksY << " blocks)" << std::endl
        << "Block size: " << Global::BLOCK_SIZE << std::endl;
}

//void Map::GenerateLocals() {
//    for (auto &vect : tiles)
//        for (auto &tile : vect)
//            tile->CheckLocal();
//
//    Server::log << "Num of locals:" << locals.size() << std::endl;
//}

//void Map::NewLocal(Tile *tile) {
//    if (!tile) return;
//    locals.push_back(uptr<Local>(new Local(tile)));
//    tile->SetLocal(locals.back().get());
//}

//void Map::RemoveLocal(const Local *local) {
//    for (auto &i : locals) {
//        if (i.get() == local) {
//            locals.remove(i);
//            break;
//        }
//    }
//}

void Map::ClearDiffs() {
    for (auto &vect : blocks)
        for (auto &block : vect)
            block->ClearDiffs();
}

void Map::Update() {
    for (auto &vect : tiles)
        for (auto &tile : vect)
            tile->Update();
}

Tile *Map::GetTile(int x, int y) const {
    if (x >= 0 && x < int(sizeX) && y >= 0 && y < int(sizeY))
        return tiles[y][x].get();
    return nullptr;
}

Block *Map::GetBlock(int x, int y) const {
    if (x >= 0 && x < int(numOfBlocksX) && y >= 0 && y < int(numOfBlocksY))
        return blocks[y][x].get();
    return nullptr;
}

int Map::GetNumOfBlocksX() const { return numOfBlocksX; }
int Map::GetNumOfBlocksY() const { return numOfBlocksY; };