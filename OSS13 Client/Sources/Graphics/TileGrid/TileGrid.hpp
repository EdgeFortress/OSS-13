#pragma once

#include <vector>
#include <unordered_map>
#include <mutex>
#include <SFML/System/Time.hpp>

#include <Graphics/UI/Widget/CustomWidget.h>
#include "Tile.hpp"

#include <Shared/Types.hpp>
#include <Shared/Network/Protocol/ServerToClient/OverlayInfo.h>

namespace sf { 
    class Event;
    class Packet;
}

class TileGrid : public CustomWidget {
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
		void RelocateObject(uint id, apos toVec, int toObjectNum);
		void SetMoveIntentObject(uint id, uf::Direction direction);
		void MoveObject(uint id, uf::Direction direction, float speed);
		void UpdateObjectIcons(uint id, const std::vector<uint32_t> &icons);
		void PlayAnimation(uint id, uint animation_id);
		void ChangeObjectDirection(uint id, uf::Direction direction);
		void Stunned(uint id, sf::Time duration);

		void ShiftBlocks(apos newFirst);

		void SetCameraPosition(apos newPos);
		void SetBlock(apos pos, std::shared_ptr<Tile>);
		void SetControllable(uint id, float speed);
		void UpdateOverlay(std::vector<network::protocol::OverlayInfo> &overlayInfo);
		void ResetOverlay();

    ////

    Tile *GetTileRel(apos) const;
    Tile *GetTileAbs(apos) const;
	int GetTileSize() const;
    Object *GetObjectUnderCursor() const;

    friend sf::Packet &operator>>(sf::Packet &packet, TileGrid &tileGrid);
    friend sf::Packet &operator>>(sf::Packet &packet, Tile &tile);
	friend std::unique_ptr<Tile> CreateTileWithInfo(TileGrid *tileGrid, const network::protocol::TileInfo &tileInfo);

protected:
    void draw() const override final;

private:
    uf::vec2i numOfTiles;
    int tileSize;

    // Camera position
    apos cameraPos;
    apos cameraRelPos;
    uf::vec2f shift;
    int cameraZ = 0;

    // TileGrid padding
    uf::vec2i padding;

    apos firstTile;
    int visibleTilesSide;
    int visibleTilesHeight;

    std::vector< sptr<Tile> > blocks;
    std::unordered_map< uint, uptr<Object> > objects;

    mutable std::mutex mutex;
    mutable std::vector< std::vector<Object *> > layersBuffer;

	bool overlayToggled;

    // Controls
    Object *controllable;
    float controllableSpeed;
    const sf::Time ACTION_TIMEOUT = sf::milliseconds(100);
    uf::vec2i moveCommand;
    int moveZCommand;
    sf::Time actionSendPause;
	sf::Time stun;

    uf::vec2i cursorPosition;
    mutable Object *underCursorObject;

	bool objectClicked;
	bool dropButtonPressed;
    bool buildButtonPressed;
    bool ghostButtonPressed;

    uint flat_index(const apos c) const;
};