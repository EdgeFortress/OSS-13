#pragma once

#include <list>

#include "Shared/Types.hpp"

class Object;
class Block;
class TileGrid;
class Sprite;

namespace sf {
	class RenderTarget;
	class Time;
	class Packet;
}

class Tile {
private:
	Block *block;
	const int x, y;
	Sprite *sprite;
	std::list<Object *> content;

public:
	Tile(Block *block, const int x, const int y);
	Tile(const Tile &) = delete;
	Tile &operator=(const Tile &) = delete;
	~Tile();

	void Draw(sf::RenderTarget *, const int x, const int y) const;
	void Update(sf::Time timeElapsed);

	void AddObject(Object *obj, int num = -1);
	Object *RemoveObject(uint id);
	Object *RemoveObject(Object *obj);
	void Clear();

	int GetRelX() const;
	int GetRelY() const;
	Object *GetObjectByCoord(const unsigned x, const unsigned y) const;
	Object *GetObject(uint id);
	bool IsBlocked();
	
	// TODO: remove this
	const std::list<Object *> &GetContent() const { return content; }

	friend sf::Packet &operator>>(sf::Packet &packet, Tile &tile);
	friend TileGrid;
};