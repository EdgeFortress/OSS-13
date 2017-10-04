#pragma once

#include <list>
#include <SFML/Graphics.hpp>

#include "Shared/Types.hpp"

class Object;
class Block;
class TileGrid;
class Sprite;

namespace sf {
    class Packet;
}

class Tile {
public:
	Tile(Block *block, const int x, const int y);
	Tile(const Tile &) = delete;
	Tile &operator=(const Tile &) = delete;
	~Tile();

	void Draw(sf::RenderTarget *, uf::vec2i windowPos) const;
	void Update(sf::Time timeElapsed);

	void AddObject(Object *obj, int num = -1);
	Object *RemoveObject(uint id);
	Object *RemoveObject(Object *obj);
	void Clear();

	uf::vec2i GetRelPos() const;
	Object *GetObjectByPixel(uf::vec2i pixel) const;
	Object *GetObject(uint id);
    TileGrid *GetTileGrid();
	bool IsBlocked();
	
	// TODO: remove this
	const std::list<Object *> &GetContent() const { return content; }

	friend sf::Packet &operator>>(sf::Packet &packet, Tile &tile);
	friend TileGrid;

private:
    Block *block;
    const uf::vec2i pos;
    Sprite *sprite;
    std::list<Object *> content;
};