#pragma once

#include <vector>
#include <unordered_map>
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
        void RelocateObject(uint id, uf::vec2i toVec, int toObjectNum);
        void SetMoveIntentObject(uint id, uf::Direction direction);
        void MoveObject(uint id, uf::Direction direction);
		void UpdateObjectIcons(uint id, const std::vector<uint32_t> &icons);
        void PlayAnimation(uint id, uint animation_id);
		void ChangeObjectDirection(uint id, uf::Direction direction);

        void ShiftBlocks(uf::vec2i newFirst);

        void SetCameraPosition(uf::vec2i newPos);
        void SetBlock(uf::vec2i pos, Block *);
        void SetControllable(uint id, float speed);

    ////

    Tile *GetTileRel(uf::vec2i) const;
    Tile *GetTileAbs(uf::vec2i) const;
	int GetBlockSize() const;
	int GetTileSize() const;
    Object *GetObjectUnderCursor() const;

    friend sf::Packet &operator>>(sf::Packet &packet, TileGrid &tileGrid);
    friend sf::Packet &operator>>(sf::Packet &packet, Tile &tile);

protected:
    void draw() const override final;

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

    std::vector< std::vector< sptr<Block> > > blocks;
    std::unordered_map< uint, uptr<Object> > objects;

    mutable std::mutex mutex;
    mutable std::vector< std::vector<Object *> > layersBuffer;

    // Controls
    Object *controllable;
    float controllableSpeed;
    const sf::Time ACTION_TIMEOUT = sf::milliseconds(100);
    uf::vec2i moveCommand;
    sf::Time actionSendPause;

    uf::vec2i cursorPosition;
    mutable Object *underCursorObject;

    bool buildButtonPressed;
    bool ghostButtonPressed;
    bool objectClicked;
};