#include <iostream>

#include "Common/Useful.hpp"
#include "Common/NetworkConst.hpp"
#include "World.hpp"
#include "Objects.hpp"
#include "Server.hpp"
#include "Player.hpp"
#include "Network/Differences.hpp"
#include "Network/TileGrid_Info.hpp"

Tile::Tile(Map *map, int x, int y) :
    map(map), x(x), y(y), local(nullptr)
{
    unsigned ux = x, uy = y;
    sprite = Global::Sprite(unsigned(Global::Sprite::Space) + ((ux + uy) ^ ~(ux * uy)) % 25);
}

void Tile::addObject(Object *obj) {
    Tile *lastTile;
    Block *lastBlock = nullptr;
    if (lastTile = obj->GetTile()) {
        for (auto iter = lastTile->content.begin(); iter != lastTile->content.end(); iter++)
            if (*iter == obj) {
                lastTile->content.erase(iter);
                break;
            }

        lastBlock = lastTile->GetBlock();
    }

    int position = 0;
    auto iter = content.begin();
    while (iter != content.end() && (*iter)->layer < obj->layer) 
        iter++, position++;
    content.insert(iter, obj);

    obj->tile = this;
}

bool Tile::RemoveObject(Object *obj) {
    for (auto &i : content)
        if (i == obj) {
            //i.release();
            i = nullptr;
            content.remove(i);
            obj->tile = nullptr;
            //add local remove
            return true;
        }
    return false;
}

void Tile::MoveTo(Object *obj) {
    bool available = true;
    for (auto &object : content)
        if (object)
            if (object->GetDensity()) {
                available = false;
                break;
            }

    if (available && obj) {
        Block *lastBlock = obj->GetTile()->GetBlock();
        int dx = X() - obj->GetTile()->X();
        int dy = Y() - obj->GetTile()->Y();
        if (abs(dx) > 1 || abs(dy) > 1)
            Server::log << "Move delta warning" << endl;
        Global::Direction direction = Global::VectToDirection(sf::Vector2i(dx, dy));
        addObject(obj);
        GetBlock()->AddDiff(new MoveDiff(obj->id, direction, lastBlock));
    }
}

void Tile::PlaceTo(Object *obj) {
    Tile *lastTile = obj->GetTile();
    Block *lastBlock = nullptr;
    if (lastTile) lastBlock = lastTile->GetBlock();
    addObject(obj);
    GetBlock()->AddDiff(new ReplaceDiff(obj->id, X(), Y(), lastBlock, obj));
}

Block *Tile::GetBlock() const {
    return map->GetBlock(x / Global::BLOCK_SIZE, y / Global::BLOCK_SIZE);
}

void Tile::CheckLocal() {
    if (local) return;

    bool floor = false;
    for (auto &obj : content) {
        if (dynamic_cast<Wall *>(obj)) {
            return;
        }
        if (dynamic_cast<Floor *>(obj)) {
            floor = true;
        }
    }
    if (!floor) return;

    for (int dx = -1; dx <= 1; dx++)
        for (int dy = -1; dy <= 1; dy++) {
            if (!map->GetTile(x + dx, y + dy) || 
                dx == 0 && dy == 0)
                continue;
            if (map->GetTile(x + dx, y + dy)->local) {
                if (local) {
                    local->Merge(map->GetTile(x + dx, y + dy)->local);
                } else {
                    map->GetTile(x + dx, y + dy)->local->AddTile(this);
                    local = map->GetTile(x + dx, y + dy)->local;
                }
            }
        }
    if (!local) {
        map->NewLocal(this);
    }
}

void Tile::Update() {
    /*for (auto iter = content.begin(); iter != content.end();) {
        auto obj = iter->get();
        if (obj)
            obj->Update();
        if (iter->get() == nullptr)
            iter = content.erase(iter);
        else
            iter++;
    }*/
}

const TileInfo Tile::GetTileInfo() const {
    std::list<ObjectInfo> content;
    for (auto &obj : this->content) {
        content.push_back(obj->GetObjectInfo());
    }
    return std::move(TileInfo(int(sprite), content));
}

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
            tile = map->GetTile(x + blockX * size, y + blockY * size);
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

const BlockInfo Block::GetBlockInfo() {
    std::list<TileInfo> tilesInfo;
    for (auto &vect : tiles)
        for (auto &tile : vect)
            if (tile) tilesInfo.push_back(tile->GetTileInfo());
            else {
                TileInfo tileInfo;
                tilesInfo.push_back(tileInfo);
            }
            return std::move(BlockInfo(blockX, blockY, tilesInfo));
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

void Map::GenerateLocals() {
    for (auto &vect : tiles)
        for (auto &tile : vect)
            tile->CheckLocal();

    Server::log << "Num of locals:" << locals.size() << endl;
}

void Map::NewLocal(Tile *tile) {
    if (!tile) return;
    locals.push_back(uptr<Local>(new Local(tile)));
    tile->SetLocal(locals.back().get());
}

void Map::RemoveLocal(const Local *local) {
    for (auto &i : locals) {
        if (i.get() == local) {
            locals.remove(i);
            break;
        }
    }
}

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
    if (x >= 0 && x < sizeX && y >= 0 && y < sizeY)
        return tiles[y][x].get();
    return nullptr;
}

Block *Map::GetBlock(int x, int y) const {
    if (x >= 0 && x < numOfBlocksX && y >= 0 && y < numOfBlocksY)
        return blocks[y][x].get();
    return nullptr;
}

int Map::GetNumOfBlocksX() const { return numOfBlocksX; }
int Map::GetNumOfBlocksY() const { return numOfBlocksY; };

void World::Update(sf::Time timeElapsed) {
    map->ClearDiffs();

	// Simple walking mob AI for moving testing
    if (testMob_lastPosition != testMob->GetTile()) {
		testMob_lastPosition = testMob->GetTile();
        int x = testMob->GetTile()->X() + test_dx;
        int y = testMob->GetTile()->Y() + test_dy;
        if (test_dx == 1 && x == 53) test_dx = 0, test_dy = 1;
        if (test_dy == 1 && y == 53) test_dx = -1, test_dy = 0;
        if (test_dx == -1 && x == 47) test_dx = 0, test_dy = -1;
        if (test_dy == -1 && y == 47) test_dx = 1, test_dy = 0;

		testMob->MoveCommand(sf::Vector2i(test_dx, test_dy));
        //map->GetTile(x, y)->AddObject(testMob);
    }
    
    map->Update();

    for (unsigned i = 0; i < objects.size();) {
        size_t len = objects.size();
        (objects.begin() + i)->get()->Update(timeElapsed);
        if (objects.size() == len)
            i++;
    }
}

void World::FillingWorld() {
    for (int i = 45; i <= 55; i++) {
        for (int j = 45; j <= 55; j++) {
            Tile *tile = map->GetTile(i, j);
            tile->PlaceTo(new Floor);
            if ((i == 45 || i == 55) && (j == 45 || j == 55))
                tile->PlaceTo(new Wall);
        }
    }

    for (int i = 5; i <= 10; i++) {
        for (int j = 5; j <= 10; j++) {
            map->GetTile(i, j)->PlaceTo(new Floor);
        }
    }

    testMob = new Mob;
	testMob_lastPosition = nullptr;
    map->GetTile(49, 49)->PlaceTo(dynamic_cast<Mob *>(testMob));
    test_dx = 1;
    test_dy = 0;

    /*for (int i = 85; i <= 95; i++) {
        for (int j = 85; j <= 95; j++) {
            Tile *tile = map->GetTile(i, j);
            new Floor(tile);
        }
    }*/
}

void World::AddObject(Object *obj) {
    objects.push_back(uptr<Object>(obj));
}

Mob *World::CreateNewPlayerMob() {
    Mob *mob = new Mob();
    map->GetTile(50, 50)->PlaceTo(mob);
    return mob;
}