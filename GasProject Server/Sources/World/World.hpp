#pragma once

#include <list>
#include <vector>

#include "Common/NetworkConst.hpp"
#include "Camera.hpp"

using std::list;
using std::vector;

class Player;

class Tile;
class Map;

class Object {
protected:
	Tile *tile;
	Global::Sprite sprite;

public:
	Object();
	explicit Object(Tile *tile = nullptr);

	Tile *GetTile() { return tile; }

	friend sf::Packet &operator<<(sf::Packet &, Object &);
};

class Item : public Object {

};

class Turf : public Object {
protected:
	bool density;

public:
    explicit Turf(Tile *tile) : Object(tile) {}
};

class Mob : public Object {
public:
    explicit Mob(Tile *tile) : Object(tile) {
        sprite = Global::Sprite::Mob;
    }
};

class Wall : public Turf {
public:
    explicit Wall(Tile *tile) : Turf(tile) {
        density = true;
        sprite = Global::Sprite::Wall;
    }

	Wall(const Wall &object) = default;
	~Wall() = default;
};

class Floor : public Turf {
public:
    explicit Floor(Tile *tile) : Turf(tile) {
        density = false;
        sprite = Global::Sprite::Floor;
    }
	
    Floor(const Floor &object) = default;
	~Floor() = default;
};

class Airlock : public Turf {
public:
    explicit Airlock(Tile *tile) : Turf(tile) {
        density = true;
        sprite = Global::Sprite::Airlock;
    }

	Airlock(const Airlock &object) = default;
	~Airlock() = default;
};

class Tile {
private:
	Map *map;
	int x, y;
    Global::Sprite sprite;

    list<uptr<Object>> content;

public:
    explicit Tile(Map *map, int x, int y);

	bool AddObject(Object *obj);
	// Removing object from tile content, but not deleting it
	bool RemoveObject(Object *obj);

    int X() const { return x; }
    int Y() const { return y; }
    Map *GetMap() const { return map; }

	friend sf::Packet &operator<<(sf::Packet &, Tile &);
};

class Block {
private:
    Map *map;
    int blockX, blockY;
    int size;

    vector< vector<Tile *> > tiles;

public:
    explicit Block(Map *map, int blockX, int blockY);

    int X() const { return blockX; }
    int Y() const { return blockY; }

	//Tile *GetTile(int x, int y);

	friend sf::Packet &operator<<(sf::Packet &, Block &);
};

class Map {
private:
    int sizeX, sizeY;
    int numOfBlocksX, numOfBlocksY;

	vector< vector<uptr<Tile>> > tiles;
    vector< vector<uptr<Block>> > blocks;

public:
    explicit Map(const int sizeX, const int sizeY);
	Tile *GetTile(int x, int y) const;
    Block *GetBlock(int x, int y) const;
};

class World {
private:
	uptr<Map> map;

public:
	World() : map(new Map(100, 100)) {
		FillingWorld();
	}

    void Update() { }
	void FillingWorld();
    Mob *CreateNewPlayerMob();
};
