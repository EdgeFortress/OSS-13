#include <iostream>

#include "Common/Useful.hpp"
#include "World.hpp"
#include "Server.hpp"

Object::Object() {
	tile = nullptr;
}

Object::Object(Tile *tile) {
	this->tile = tile;
    if (tile)
	    tile->AddObject(this);
}

Tile::Tile(Map *map, int x, int y) : content() {
	this->map = map;
	this->x = x;
	this->y = y;
}

bool Tile::AddObject(Object *obj) {
    Server::log << content.size() << std::endl;
	for (auto &i : content)
        if (typeid(i.get()) == typeid(obj)) {
            return false;
        }
	content.push_back(uptr<Object>(obj));
    return true;
}

bool Tile::RemoveObject(Object *obj) {
	for (auto &i : content)
        if (i.get() == obj) {
            content.remove(i);
            return true;
        }
    return false;
}

Map::Map(const int sizeX, const int sizeY) : tiles(sizeY) {
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
}

Tile *Map::GetTile(int x, int y) const {
	if (x >= 0 && x < sizeX && y >= 0 && y < sizeY) 
        return tiles[y][x].get();
	return nullptr;
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
}