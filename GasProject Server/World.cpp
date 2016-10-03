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
	content = new list<Object *>();
}

void Tile::AddObject(Object *obj) {
	content->push_back(obj);
}

void Tile::RemoveObject(Object *obj) {
	content->remove(obj);
}

Tile::~Tile() {
	for (Object *obj : *content)
		delete obj;
	delete content;
}

Map::Map() {
	sizeX = 100; sizeY = 100;

	tiles = new vector< vector<Tile *> * >(sizeY);
	for (vector<Tile *> *& vect : *tiles) {
		vect = new vector<Tile *>(sizeX);
	}

	int y = 0;
	for (vector<Tile *> *& vect : *tiles) {
		int x = 0;
		for (Tile *& tile : *vect) {
			tile = new Tile(this, x, y);
			x++;
		}
		y++;
	}
}

Tile *Map::GetTile(int x, int y) {
	if (x >= 0 && x < sizeX && y >= 0 && y < sizeY) return (*(*tiles)[y])[x];
	return nullptr;
}

Map::~Map() {
	for (vector<Tile *> *& vect : *tiles)
	{
		for (Tile *& tile : *vect)
			delete tile;
		delete vect;
	}
	delete tiles;
}