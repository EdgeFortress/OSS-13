#include <iostream>

#include "Common/Useful.hpp"
#include "Common/NetworkConst.hpp"
#include "World.hpp"
#include "Server.hpp"
#include "Player.hpp"

Object::Object() {
    tile = nullptr;
}

Object::Object(Tile *tile) : tile(nullptr) {
    if (tile)
        tile->AddObject(this);
}

void Mob::Update() {
    if (vertical < 0)
        tile->MoveTo(this, tile->GetMap()->GetTile(tile->X(), tile->Y() - 1)), vertical++;
    if (vertical > 0)
        tile->MoveTo(this, tile->GetMap()->GetTile(tile->X(), tile->Y() + 1)), vertical--;
    if (horizontal < 0)
        tile->MoveTo(this, tile->GetMap()->GetTile(tile->X() - 1, tile->Y())), horizontal++;
    if (horizontal > 0)
        tile->MoveTo(this, tile->GetMap()->GetTile(tile->X() + 1, tile->Y())), horizontal--;
}

Tile::Tile(Map *map, int x, int y) :
    map(map), x(x), y(y), local(nullptr)
{
    unsigned ux = x, uy = y;
    sprite = Global::Sprite(unsigned(Global::Sprite::Space) + ((ux + uy) ^ ~(ux * uy)) % 25);
}

void Tile::AddObject(Object *obj) {
    Tile *lastTile;
    bool moved = false;
    if (lastTile = obj->GetTile()) {
        int n = 0;
        for (auto &i : lastTile->content) {
            if (i.get() == obj) {
                i.release();
                lastTile->content.remove(i);

                Block *block = lastTile->GetBlock();
                int x = lastTile->X() % Global::BLOCK_SIZE;
                int y = lastTile->Y() % Global::BLOCK_SIZE;
                if (GetBlock() == block) {
                    int toX = X() % Global::BLOCK_SIZE;
                    int toY = Y() % Global::BLOCK_SIZE;
                    block->AddDiff(new MoveDiff(block, x, y, n, toX, toY));
                    moved = true;
                } else {
                    block->AddDiff(new RemoveDiff(block, x, y, n));
                }
                
                break;
            }
            n++;
        }
    }
    obj->SetTile(this);
    content.push_back(uptr<Object>(obj));

    if (!moved) {
        Block *block = GetBlock();
        int x = X() % Global::BLOCK_SIZE;
        int y = Y() % Global::BLOCK_SIZE;
        block->AddDiff(new AddDiff(block, x, y, int(content.size() - 1)));
    }
}

bool Tile::RemoveObject(Object *obj) {
    for (auto &i : content)
        if (i.get() == obj) {
            i.release();
            content.remove(i);
            obj->SetTile(nullptr);
            //add local remove
            return true;
        }
    return false;
}

void Tile::MoveTo(Object *obj, Tile *tile) {
    bool available = true;
    for (auto &object : tile->GetContent())
        if (object->GetDensity())
        {
            available = false;
            break;
        }
    
    if (available)
    {
        RemoveObject(obj);
        tile->AddObject(obj);
    }
}

Block *Tile::GetBlock() const {
    return map->GetBlock(x / Global::BLOCK_SIZE, y / Global::BLOCK_SIZE);
}

void Tile::CheckLocal() {
    if (local) return;

    bool floor = false;
    for (auto &obj : content) {
        if (dynamic_cast<Wall *>(obj.get())) {
            return;
        }
        if (dynamic_cast<Floor *>(obj.get())) {
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

void Block::ClearDiffs() {
    differences.clear();
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

    time_since_testMob_update += timeElapsed;
    if (time_since_testMob_update >= sf::seconds(1)) {
        int x = testMob->GetTile()->X() + test_dx;
        int y = testMob->GetTile()->Y() + test_dy;
        if (test_dx == 1 && x == 51) test_dx = 0, test_dy = 1;
        if (test_dy == 1 && y == 51) test_dx = -1, test_dy = 0;
        if (test_dx == -1 && x == 49) test_dx = 0, test_dy = -1;
        if (test_dy == -1 && y == 49) test_dx = 1, test_dy = 0;

        map->GetTile(x, y)->AddObject(testMob);
        time_since_testMob_update = sf::Time::Zero;
    }

    mobsUpdateTime += timeElapsed;
    if (mobsUpdateTime >= sf::seconds(float(1.0 / mobsVelocity))) {
        mobsUpdateTime = sf::Time::Zero;
        for (auto &vect : map->GetTiles())
            for (auto &tile : vect)
                for (auto &obj : tile->GetContent())
                    obj->Update();
    }
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

    for (int i = 5; i <= 10; i++) {
        for (int j = 5; j <= 10; j++) {
            Tile *tile = map->GetTile(i, j);
            new Floor(tile);
        }
    }

    /*for (int i = 85; i <= 95; i++) {
        for (int j = 85; j <= 95; j++) {
            Tile *tile = map->GetTile(i, j);
            new Floor(tile);
        }
    }*/
}

Mob *World::CreateNewPlayerMob() {
    Tile *startTile = map->GetTile(50, 50);
    return new Mob(startTile);
}