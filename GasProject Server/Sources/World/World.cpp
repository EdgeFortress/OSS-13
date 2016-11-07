#include <iostream>

#include "Common/Useful.hpp"
#include "Common/NetworkConst.hpp"
#include "World.hpp"
#include "Server.hpp"
#include "Player.hpp"

Object::Object() {
	tile = nullptr;
}

Object::Object(Tile *tile) {
	this->tile = tile;
    if (tile)
	    tile->AddObject(this);
}

Tile::Tile(Map *map, int x, int y) :
    map(map), x(x), y(y)
{
    sprite = Global::Sprite::Space;
}

bool Tile::AddObject(Object *obj) {
	for (auto &i : content)
        if (typeid(i.get()) == typeid(obj)) {
            return false;
        }
	content.push_back(uptr<Object>(obj));
    return true;
}

bool Tile::RemoveObject(Object *obj) {
	for (auto &i : content)
        if (i.get() == obj) {
            content.remove(i);
            return true;
        }
    return false;
}

Block::Block(Map *map, int blockX, int blockY) :
    map(map), blockX(blockX), blockY(blockY),
    size(Global::BLOCK_SIZE),
    tiles(size, vector<Tile *>(size))
    
{
    int y = 0;
    for (vector<Tile *> &vect : tiles) {
        int x = 0;
        for (Tile *&tile : vect) {
            tile = map->GetTile(x + blockX * size, y + blockY * size);
            x++;
        }
        y++;
    }
}

Map::Map(const int sizeX, const int sizeY) : 
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

Tile *Map::GetTile(int x, int y) const {
	if (x >= 0 && x < sizeX && y >= 0 && y < sizeY) 
        return tiles[y][x].get();
	return nullptr;
}

Block *Map::GetBlock(int x, int y) const {
    if (x >= 0 && x < numOfBlocksX && y >= 0 && y < numOfBlocksY)
        return blocks[y][x].get();
    return nullptr;
}

void World::FillingWorld() {
    for (int i = 45; i <= 55; i++) {
        for (int j = 45; j <= 55; j++) {
            Tile *tile = map->GetTile(i, j);
            new Floor(tile);
            if ((i == 45 || i == 55) && (j == 45 || j == 55))
                new Wall(tile);
        }
    }
}

Mob *World::CreateNewPlayerMob() {
    Tile *startTile = map->GetTile(50, 50);
    return new Mob(startTile);
}