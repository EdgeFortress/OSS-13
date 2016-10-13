#pragma once
#include <SFML/Graphics.hpp>

#include <list>
#include <vector>

using std::list;
using std::vector;

class Tile;
class Map;

class Object {
protected:
	Tile *tile;
public:
	Object();
	Object(Tile *tile);
};

class Item : public Object {

};

class Block : public Object {
protected:
	bool density;
public:
	Block(bool density) : density(density){}
};

class Mob : public Object {

};

class Wall : public Block {
public:
	Wall(bool density = true) : Block(density){}
	Wall(const Wall &object);

	~Wall();
};

class Floor : public Block {
public:
	Floor(bool density = false) : Block(density) {}
	Floor(const Floor &object);

	~Floor();
};

class Gate : public Block {
public:
	Gate(bool density = true) : Block(density) {}
	Gate(const Gate &object);

	~Gate();

};

class Tile {
private:
	list<Object *> content;
	Map *map;
	int x, y;

public:
	Tile(Map *map, int x, int y);
	//add object to map
	void AddObject(Object *obj);
	// Removing object from tile content, but not deleting it
	void RemoveObject(Object *obj);
	~Tile();
};

class Map {
private:
	vector< vector<Tile *> > tiles;
	int sizeX;
	int sizeY;

public:
	Map(int x = 100, int y = 100);
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
	//filling the world
	void FillingWorld();
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