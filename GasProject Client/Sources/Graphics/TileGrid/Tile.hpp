#pragma once

#include <list>
#include <SFML/Graphics.hpp>

#include "Graphics/Sprite.hpp"
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
	void DrawOverlay(sf::RenderTarget *, uf::vec2i windowPos) const;
	void Update(sf::Time timeElapsed);
    void Resize(uint tileSize);

	void AddObject(Object *obj, int num = -1);
	Object *RemoveObject(uint id);
	Object *RemoveObject(Object *obj);
	void Clear();

	void SetOverlay(std::string text);

	uf::vec2i GetRelPos() const;
	Object *GetObject(uint id);
    TileGrid *GetTileGrid();
	bool IsBlocked();

	friend sf::Packet &operator>>(sf::Packet &packet, Tile &tile);
	friend TileGrid;

private:
    Block *block;
    const uf::vec2i pos;
    ::Sprite sprite;
    std::list<Object *> content;
	mutable sf::Text overlay;
};
