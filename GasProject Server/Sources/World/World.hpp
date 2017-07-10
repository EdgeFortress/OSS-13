#pragma once

#include <string>
#include <list>
#include <vector>

#include <SFML/System/Time.hpp>

#include "Objects.hpp"
#include "Common/Useful.hpp"

using std::list;
using std::vector;

class Player;
struct Diff;

class Block;
class Map;
class Gas;
class Local;

struct TileInfo;
struct BlockInfo;

class Tile {
private:
    Map *map;
    int x, y;
    Global::Sprite sprite;
    Local *local;

    list<Object *> content;
    list<Gas> listGas;

    // Add object to the tile, and change object.tile pointer
    // If object was in content of other tile, generate MoveDiff,
    // else generate AddDiff
    void addObject(Object *obj);

public:
    explicit Tile(Map *map, int x, int y);

    void CheckLocal();
    void Update();

    // Removing object from tile content, but not deleting it, and change object.tile pointer
    // Also generate DeleteDiff
    bool RemoveObject(Object *obj);
    void MoveTo(Object *);
    void PlaceTo(Object *);

    int X() const { return x; }
    int Y() const { return y; }
    Block *GetBlock() const;
    Map *GetMap() const { return map; }

    bool IsDense() {
        for (auto &obj : content)
            if (obj->GetDensity()) return true;
        return false;
    }

    void SetLocal(Local *local) { this->local = local; }

    //Test
    const list<Object *> GetContentCopy() { return content; };

    const TileInfo GetTileInfo() const;

    friend sf::Packet &operator<<(sf::Packet &, const Tile &);
};

class Block {
private:
    Map *map;
    int id;
    int blockX, blockY;
    int size;

    vector< vector<Tile *> > tiles;
    list<sptr<Diff>> differences;

public:
    explicit Block(Map *map, int blockX, int blockY);

    void AddDiff(Diff *diff);

    Tile *GetTile(int x, int y) const { return tiles[y][x]; }
    int X() const { return blockX; }
    int Y() const { return blockY; }
    int ID() const { return id; }

    const list<sptr<Diff>> GetDifferences() { return differences; }
    const BlockInfo GetBlockInfo();
    void ClearDiffs();

    friend sf::Packet &operator<<(sf::Packet &, const Block &);
};

class Map {
private:
    int sizeX, sizeY;
    int numOfBlocksX, numOfBlocksY;

    vector< vector<uptr<Tile>> > tiles;
    vector< vector<uptr<Block>> > blocks;
    list<uptr<Local>> locals;

public:
    explicit Map(const int sizeX, const int sizeY);

    void GenerateLocals();

    void NewLocal(Tile *tile);
    void RemoveLocal(const Local *local);

    void ClearDiffs();
    void Update();

    Tile *GetTile(int x, int y) const;
    Block *GetBlock(int x, int y) const;
    int GetNumOfBlocksX() const;
    int GetNumOfBlocksY() const;
};

class World {
private:
    uptr<Map> map;

    Human *testMob;
	Tile *testMob_lastPosition;
    int test_dx;
    int test_dy;
    int mobsVelocity;

    std::vector<uptr<Object>> objects;

public:
    World() : map(new Map(100, 100)) {
        map->GenerateLocals();
    }

    void Update(sf::Time timeElapsed);

    void AddObject(Object *);

    void FillingWorld();
    Creature *CreateNewPlayerCreature();
};

class Local {
    list<Tile *> tiles;
    Map *map;

public:
    Local(Tile *tile) : map(tile->GetMap()) { tiles.push_back(tile); }

    void AddTile(Tile *tile) {
        tiles.push_back(tile);
    }
    void Merge(Local *local) {
        if (!local) return;
        if (this == local) return;
        for (auto &tile : local->tiles) {
            tiles.push_back(tile);
            tile->SetLocal(this);
        }
        map->RemoveLocal(local);
    }
    //test
    const list<Tile *> &GetTiles() { return tiles; }
};

class Gas {
    double pressure;
};

class Oxygen : public Gas {

};
