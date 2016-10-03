#pragma once

#include <list>
#include <vector>

using std::list;
using std::vector;

class Tile;
class Map;

class Object {
private:
	Tile *tile;

public:
	Object();
	Object(Tile *tile);
};

class Item : public Object {

};

class Block : public Object {

};

class Mob : public Object {

};

class Tile {
private:
	list<Object *> *content;
	Map *map;
	int x, y;

public:
	Tile(Map *map, int x, int y);
	void AddObject(Object *obj);
	// Removing object from tile content, but not delete it
	void RemoveObject(Object *obj);
	~Tile();
};

class Map {
private:
	vector< vector<Tile *> * > *tiles;
	int sizeX;
	int sizeY;

public:
	Map();
	Tile *GetTile(int x, int y);
	~Map();
};

class World {
private:
	Map *map;

public:
	World() {
		map = new Map();
	}

	~World() {
		delete map;
	}
};

class Camera {
private:
	Tile *tile;
	bool suspense;

public:
	Camera() {
		suspense = true;
		tile = nullptr;
	}

	Camera(Tile *tile) {
		this->tile = tile;
		suspense = false;
	}

	void SetPosition(Tile *tile) {
		this->tile = tile;
		suspense = false;
	}

	void Suspend() {
		tile = nullptr;
		suspense = true;
	}
};