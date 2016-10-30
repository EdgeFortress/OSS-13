#include <iostream>

#include "useful.hpp"
#include "World.hpp"

Object::Object() {
	tile = nullptr;
}

Object::Object(Tile *tile) {
	this->tile = tile;
	tile->AddObject(this);
}


Tile::Tile(Map *map, int x, int y) {
	this->map = map;
	this->x = x;
	this->y = y;
	content = list<Object *>();
}

void Tile::AddObject(Object *obj) {
	for (auto i : content) {
		if (typeid(i) == typeid(obj)) {
			Log log;
			//log << "this type of object already exists" << std::endl;
			return;
		}
	}
	content.push_back(obj);
}

void Tile::RemoveObject(Object *obj) {
	bool flag = false;
	for (auto i : content) {
		if (typeid(i) == typeid(obj))
			flag = true;
	}
	if (flag) {
		content.remove(obj); 
    } else {
		Log log;
		log << "this type of object does not exist in this tile" << std::endl;
		return;
	}
}

Tile::~Tile() {
	for (Object *obj : content)
		delete obj;
}

Map::Map(int q, int p) {
	sizeX = q; sizeY = p;

	tiles = vector< vector<Tile *> >(sizeY, vector<Tile *>(sizeX));

	int y = 0;
	for (vector<Tile *> &vect : tiles) {
		int x = 0;
		for (Tile * &tile : vect) {
			tile = new Tile(this, x, y);
			x++;
		}
		y++;
	}
}

Tile *Map::GetTile(int x, int y) {
	if (x >= 0 && x < sizeX && y >= 0 && y < sizeY) return tiles[y][x];
	return nullptr;
}

Map::~Map() {
	for (vector<Tile *> &vect : tiles)
		for (Tile *& tile : vect)
			delete tile;
}

void World::FillingWorld() {
	Log log;
	if (map == nullptr) {
		log << "map is not created, i wil do it myself :)" << std::endl;
		map = new Map();
	}
	for (int i = 10; i < 90; i++) {
		for (int j = 10; j < 90; j++) {
			Tile *tile = map->GetTile(i, j);
			tile->AddObject(new Floor());
			if (j == 90 && i == 10 || i == 20 || i == 45 || i == 85) {
				tile->AddObject(new Gate());
			}
		}
	}
	for (int j = 10; j < 50; j++) {
		Tile *tile = map->GetTile(50, j);
		tile->AddObject(new Wall());
	}
	for (int i = 50; i < 85; i++) {
		Tile *tile = map->GetTile(i, 60);
		tile->AddObject(new Wall());
	}

}