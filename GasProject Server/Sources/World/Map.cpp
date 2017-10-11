#include "Map.hpp"

#include "Server.hpp"
#include "Tile.hpp"
#include "Block.hpp"
#include "Atmos/Atmos.hpp"
#include "Shared/Global.hpp"

Map::Map(const uint sizeX, const uint sizeY) :
    size(sizeX, sizeY),
    numOfBlocks(sizeX / Global::BLOCK_SIZE + (sizeX % Global::BLOCK_SIZE ? 1 : 0),
                sizeY / Global::BLOCK_SIZE + (sizeY % Global::BLOCK_SIZE ? 1 : 0)),
    tiles(sizeY),
    blocks(numOfBlocks.y)
{
    for (vector<uptr<Tile>> &vect : tiles) {
        vect = vector<uptr<Tile>>(sizeX);
    }

    int y = 0;
    for (vector<uptr<Tile>> &vect : tiles) {
        int x = 0;
        for (uptr<Tile> &tile : vect) {
            tile.reset(new Tile(this, {x, y}));
            x++;
        }
        y++;
    }

    for (vector<uptr<Block>> &vect : blocks) {
        vect = vector<uptr<Block>>(numOfBlocks.y);
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

    Server::log << "Map created: " << sizeX << "x" << sizeY << " (" << numOfBlocks.x << "x" << numOfBlocks.y << " blocks)" << std::endl
        << "Block size: " << Global::BLOCK_SIZE << std::endl;

    atmos = std::make_unique<Atmos>(this);
}

void Map::ClearDiffs() {
    for (auto &vect : blocks)
        for (auto &block : vect)
            block->ClearDiffs();
}

void Map::Update(sf::Time timeElapsed) {
    for (auto &vect : tiles)
        for (auto &tile : vect)
            tile->Update(timeElapsed);
    atmos->Update(timeElapsed);
}

vec2u Map::GetSize() const { return size; }
int Map::GetNumOfBlocksX() const { return numOfBlocks.x; }
int Map::GetNumOfBlocksY() const { return numOfBlocks.y; }
Atmos* Map::GetAtmos() const { return atmos.get(); };

Tile *Map::GetTile(uf::vec2i pos) const {
    if (pos >= uf::vec2i() && pos < size)
        return tiles[pos.y][pos.x].get();
    return nullptr;
}

Block *Map::GetBlock(uf::vec2i pos) const {
    if (pos >= uf::vec2i() && pos < numOfBlocks)
        return blocks[pos.y][pos.x].get();
    return nullptr;
}

const vector< vector< uptr<Tile>>>& Map::GetTiles() const { return tiles; }