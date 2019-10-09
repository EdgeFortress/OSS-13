#pragma once

#include <vector>
#include <unordered_map>
#include <SFML/System/Time.hpp>

#include <Graphics/UI/Widget/Container.hpp>
#include "Tile.hpp"
#include "ControlUI.h"

#include <Shared/Types.hpp>
#include <Shared/Network/Protocol/ServerToClient/OverlayInfo.h>
#include <Shared/Network/Protocol/ServerToClient/Diff.h>

#include <Shared/Grid.hpp>

namespace sf { 
    class Event;
    class Packet;
}

class TileGrid : public Container {
public:
	TileGrid();
	TileGrid(const TileGrid &) = delete;
	TileGrid &operator=(const TileGrid &) = delete;
	~TileGrid() = default;

	void Update(sf::Time timeElapsed) override final;

	bool OnMouseButtonPressed(sf::Mouse::Button button, uf::vec2i position) final;
	bool OnMouseMoved(uf::vec2i position) final;
	bool OnMouseLeft() final;
	bool OnMouseWheelScrolled(float delta, uf::vec2i position) final;
	bool OnKeyPressed(sf::Event::KeyEvent keyEvent) final;

	void SetSize(const uf::vec2i &windowSize) final;

    //// FOR NETWORK

		// Differences commiting
		void AddObject(Object *object);
		void RemoveObject(uint id);
		void AmendObjectChanges(network::protocol::FieldsDiff &&diff);
		void RelocateObject(uint id, apos toVec, int toObjectNum);
		void SetMoveIntentObject(uint id, uf::Direction direction);
		void MoveObject(uint id, uf::Direction direction, float speed);
		void UpdateObjectIcons(uint id, const std::vector<uint32_t> &icons);
		void PlayAnimation(uint id, uint animation_id);
		void Stunned(uint id, sf::Time duration);

		void ShiftBlocks(apos newFirst);

		void SetCameraPosition(apos newPos);
		void SetBlock(apos pos, std::shared_ptr<Tile>);
		void UpdateControlUI(const std::vector<network::protocol::ControlUIData> &elements, bool clear);
		void SetControllable(uint id, float speed);
		void UpdateOverlay(std::vector<network::protocol::OverlayInfo> &overlayInfo);
		void ResetOverlay();

	void SetFOV(int fov, int fovZ);
	int GetFOV();
	int GetFOVZ();
    ////

    Tile *GetTileRel(apos) const;
    Tile *GetTileAbs(apos) const;
	int GetTileSize() const;
    Object *GetObjectUnderCursor() const;

	std::unordered_map< uint, uptr<Object> > &GetObjects(); // TODO: remove this

protected:
	void drawContainer() const override final;

private:
	Tile *countTileUnderCursor(uf::vec2i mousePosition);

private:
    uf::vec2i numOfTiles;
    int tileSize;

    // Camera position
    apos cameraPos;
    apos cameraRelPos;
    uf::vec2f shift;
    int cameraZ = 0;

	int fov{0};
	int fovZ{0};

    apos firstTile;
    uint32_t visibleTilesSide;
    uint32_t visibleTilesHeight;

    uf::Grid< sptr<Tile> > blocks;
    std::unordered_map< uint, uptr<Object> > objects;

    mutable std::vector< std::vector<Object *> > layersBuffer;

	bool overlayToggled;

	ControlUI *controlUI;

    // Controls
    Object *controllable;
    float controllableSpeed;
    const sf::Time ACTION_TIMEOUT = sf::milliseconds(100);
    uf::vec2i moveCommand;
    int moveZCommand;
    sf::Time actionSendPause;
	sf::Time stun;

    uf::vec2i cursorPosition;
	mutable Object *underCursorObject{};
	mutable Tile *underCursorTile{};

	bool objectClicked;
	bool rmbClicked{false};
	bool dropButtonPressed;
    bool buildButtonPressed;
    bool ghostButtonPressed;

	bool movementPredictionDisabled{false};

	void updatePos(uf::vec3u pos, uf::vec3u newpos);
};
