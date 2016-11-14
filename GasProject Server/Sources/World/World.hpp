#pragma once

#include <list>
#include <vector>

#include <SFML/System/Time.hpp>

#include "Common/NetworkConst.hpp"
#include "Common/Differences.hpp"
#include "Camera.hpp"

using std::list;
using std::vector;

class Player;

class Tile;
class Map;

class Gas;
class Local;

class Object {
protected:
    Tile *tile;
    Global::Sprite sprite;
    bool density;

public:
    Object();
    // Construct object at tile
    explicit Object(Tile *tile = nullptr);

    bool GetDensity() { return density; }
    Tile *GetTile() { return tile; }
    // Just set tile pointer
    void SetTile(Tile *tile) { this->tile = tile; }
    virtual void Interact(Object *) {}

    virtual void Update() {}

    friend sf::Packet &operator<<(sf::Packet &, const Object &);
};

class Item : public Object {

};

class Turf : public Object {
public:
    explicit Turf(Tile *tile) : Object(tile) {}
};

class Mob : public Object {
private:
    int moveY, moveX;
public:
    explicit Mob(Tile *tile) : Object(tile) {
        sprite = Global::Sprite::Mob;
        density = false;
        moveY = 0; moveX = 0;
    }

    void MoveNorth() { moveY = -1; }
    void MoveSouth() { moveY = 1; }
    void MoveEast() { moveX = 1 ; }
    void MoveWest() { moveX = -1; }

    virtual void Update() override;
};

class Wall : public Turf {
public:
    explicit Wall(Tile *tile) : Turf(tile) {
        sprite = Global::Sprite::Wall;
        density = true;
    }

    Wall(const Wall &object) = default;
    ~Wall() = default;
};

class Floor : public Turf {
public:
    explicit Floor(Tile *tile) : Turf(tile) {
        sprite = Global::Sprite::Floor;
        density = false;
    }
    
    Floor(const Floor &object) = default;
    ~Floor() = default;
};

class Airlock : public Turf {
public:
    explicit Airlock(Tile *tile) : Turf(tile) {
        sprite = Global::Sprite::Airlock;
        density = true;
    }

    Airlock(const Airlock &object) = default;
    ~Airlock() = default;
};

class Tile {
private:
    Map *map;
    int x, y;
    Global::Sprite sprite;
    Local *local;

    list<uptr<Object>> content;
    list<Gas> listGas;

public:
    explicit Tile(Map *map, int x, int y);

    void CheckLocal();
    void Update();

    // Add object to the tile, and change object.tile pointer
    // If object was in content of other tile, generate MoveDiff,
    // else generate AddDiff
    void AddObject(Object *obj);
    // Removing object from tile content, but not deleting it, and change object.tile pointer
    // Also generate DeleteDiff
    bool RemoveObject(Object *obj);
    void MoveTo(Object *);

    int X() const { return x; }
    int Y() const { return y; }
    Block *GetBlock() const;
    Map *GetMap() const { return map; }

    void SetLocal(Local *local) { this->local = local; }

    //Test
    list<uptr<Object>> &GetContent() { return content; };

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

    void AddDiff(Diff *diff) {
        differences.push_back(sptr<Diff>(diff));
    }

    Tile *GetTile(int x, int y) const { return tiles[y][x]; }
    int X() const { return blockX; }
    int Y() const { return blockY; }
    int ID() const { return id; }

    const list<sptr<Diff>> GetDifferences() { return differences; }
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

    Tile *GetTile(int x, int y) const;
    Block *GetBlock(int x, int y) const;
    int GetNumOfBlocksX() const;
    int GetNumOfBlocksY() const;
};

class World {
private:
    uptr<Map> map;

    Mob* testMob;
    sf::Time time_since_testMob_update;
    sf::Time mobsUpdateTime;
    int test_dx;
    int test_dy;
    int mobsVelocity;

public:
    World() : map(new Map(100, 100)) {
        FillingWorld();
        map->GenerateLocals();

        time_since_testMob_update = sf::Time::Zero;
        testMob = new Mob(map->GetTile(49, 49));
        test_dx = 1;
        test_dy = 0;
    }

    void Update(sf::Time timeElapsed);

    void FillingWorld();
    Mob *CreateNewPlayerMob();
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
