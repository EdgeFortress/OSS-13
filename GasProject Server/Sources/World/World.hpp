#pragma once

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
	Object(Tile *tile = nullptr);
};

class Item : public Object {

};

class Turf : public Object {
protected:
	bool density;

public:
	Turf(Tile *tile) : Object(tile) {}
};

class Mob : public Object {

};

class Wall : public Turf {
public:
	Wall(Tile *tile = nullptr) : Turf(tile) {
        density = true;
    }

	Wall(const Wall &object) = default;
	~Wall() = default;
};

class Floor : public Turf {
public:
	Floor(Tile *tile) : Turf(tile) {
        density = false;
    }
	
    Floor(const Floor &object) = default;
	~Floor() = default;
};

class Gate : public Turf {
public:
	Gate(Tile *tile) : Turf(tile) {
        density = true;
    }

	Gate(const Gate &object) = default;
	~Gate() = default;
};

class Tile {
private:
	list<uptr<Object>> content;
	Map *map;
	int x, y;

public:
	Tile(Map *map, int x, int y);

	bool AddObject(Object *obj);
	// Removing object from tile content, but not deleting it
	bool RemoveObject(Object *obj);
};

class Map {
private:
	vector< vector<uptr<Tile>> > tiles;
	int sizeX;
	int sizeY;

public:
	Map(const int sizeX, const int sizeY);
	Tile *GetTile(int x, int y) const;
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