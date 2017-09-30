#pragma once

#include <list>
#include <vector>
#include <mutex>
#include <SFML/System.hpp>

#include "Shared/Geometry/Direction.hpp"
#include "Graphics/UI/Widget/Widget.hpp"

class Object;
class Tile;
class Block;

namespace sf {
	class Event;
	class RenderWindow;
	class Packet;
}

class TileGrid : public Widget {
private:
    int xNumOfTiles, yNumOfTiles;
    int tileSize;

    // Camera position
    int xPos, yPos;
    int xRelPos, yRelPos;
    sf::Vector2f shift;
    
    // TileGrid padding
    int xPadding, yPadding;

    int blockSize;
    int firstBlockX, firstBlockY;
    int firstTileX, firstTileY;
    int numOfVisibleBlocks;

    mutable std::mutex mutex;

    std::vector< std::vector< sptr<Block> > > blocks;
    std::list< uptr<Object> > objects;

    // Controls
    Object *controllable;
    float controllableSpeed;
    const sf::Time ACTION_TIMEOUT = sf::milliseconds(100);
    sf::Vector2i moveCommand;
    sf::Time actionSendPause;

    Object *clickedObject;

	bool buildButtonPressed;
	bool ghostButtonPressed;

protected:
	void draw() const override final;

public:
    TileGrid();
    TileGrid(const TileGrid &) = delete;
    TileGrid &operator=(const TileGrid &) = delete;
    ~TileGrid() = default;

    //void Draw(sf::RenderWindow * const);
	bool HandleEvent(sf::Event event) override final;
	void Update(sf::Time timeElapsed) override final;

	void SetSize(const sf::Vector2f &) override final;

    //// FOR NETWORK

        // Lock for network updating
        void LockDrawing();
        // Unlock for network updating
        void UnlockDrawing();

        // Differences commiting
        void AddObject(Object *object);
        void RemoveObject(uint id);
        void RelocateObject(uint id, int toX, int toY, int toObjectNum);
        void MoveObject(uint id, uf::Direction direction, float speed);
		void ChangeObjectDirection(uint id, uf::Direction direction);

        void ShiftBlocks(const int newFirstX, const int newFirstY);

        void SetCameraPosition(const int x, const int y);
        void SetBlock(int x, int y, Block *);
        void SetControllable(uint id, float speed);

    ////

    //wptr<Block> GetBlock(const int blockX, const int blockY) const;
    Tile *GetTileRel(int x, int y) const;
    Tile *GetTileAbs(int x, int y) const;
    Object *GetObjectByPixel(int x, int y) const;

	const int GetBlockSize() const;
	const int GetTileSize() const;
	const int GetWidth() const;
	const int GetHeigth() const;
	const int GetPaddingX() const;
	const int GetPaddingY() const;

    friend sf::Packet &operator>>(sf::Packet &packet, TileGrid &tileGrid);
    friend sf::Packet &operator>>(sf::Packet &packet, Tile &tile);
};