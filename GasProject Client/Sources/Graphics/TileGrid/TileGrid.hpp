#pragma once

#include <list>
#include <vector>
#include <mutex>
#include "SFML/System/Time.hpp" 

#include "Shared/Types.hpp"
#include "Graphics/UI/Widget/Widget.hpp"
#include "Block.hpp"

namespace sf { 
    class Event;
    class Packet;
}

class TileGrid : public Widget {
private:
    uf::vec2i numOfTiles;
    int tileSize;

    // Camera position
    uf::vec2i cameraPos;
    uf::vec2i cameraRelPos;
    uf::vec2f shift;
    
    // TileGrid padding
    uf::vec2i padding;

    int blockSize;
    uf::vec2i firstBlock;
    uf::vec2i firstTile;
    int numOfVisibleBlocks;

    mutable std::mutex mutex;

    std::vector< std::vector< sptr<Block> > > blocks;
    std::list< uptr<Object> > objects;

    // Controls
    Object *controllable;
    float controllableSpeed;
    const sf::Time ACTION_TIMEOUT = sf::milliseconds(100);
    uf::vec2i moveCommand;
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

	void SetSize(const uf::vec2i &) override final;

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
    Tile *GetTileRel(uf::vec2i) const;
    Tile *GetTileAbs(uf::vec2i) const;
    Object *GetObjectByPixel(uf::vec2i) const;

	const int GetBlockSize() const;
	const int GetTileSize() const;

    friend sf::Packet &operator>>(sf::Packet &packet, TileGrid &tileGrid);
    friend sf::Packet &operator>>(sf::Packet &packet, Tile &tile);
};