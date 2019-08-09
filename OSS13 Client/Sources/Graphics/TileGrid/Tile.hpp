#pragma once

#include <list>
#include <SFML/Graphics.hpp>

#include "Graphics/Sprite.hpp"
#include "Shared/Types.hpp"
#include <Shared/Network/Protocol/ServerToClient/WorldInfo.h>
#include <Shared/IFaces/INonCopyable.h>

class Object;
class Block;
class TileGrid;
class Sprite;

namespace sf {
    class Packet;
}

class Tile : public INonCopyable {
public:
	explicit Tile(TileGrid *tileGrid);
	Tile(Tile &&) = default;
	Tile &operator=(Tile &&) = default;
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

	apos GetRelPos() const;
	Object *GetObject(uint id);
    TileGrid *GetTileGrid();
	bool IsBlocked();

	friend sf::Packet &operator>>(sf::Packet &packet, Tile &tile);
	friend std::unique_ptr<Tile> CreateTileWithInfo(TileGrid *tileGrid, const network::protocol::TileInfo &tileInfo);
	friend TileGrid;

private:
    TileGrid *tileGrid;
    apos relPos;
    ::Sprite sprite;
    std::list<Object *> content;
	mutable sf::Text overlay;
};
