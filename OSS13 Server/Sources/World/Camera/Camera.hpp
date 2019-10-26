#pragma once

#include <unordered_set>

#include <Shared/Types.hpp>
#include <Shared/Grid.hpp>

#include "ICameraOverlay.h"

class Tile;
class Object;
class Player;
struct Diff;

namespace sf {
	class Packet;
}

struct ContextMenuNodeCache {
	Object *obj;
	std::vector<std::string> verbs;
};

class Camera {
public:
	explicit Camera(const Tile * const tile = nullptr);

	void Update(std::chrono::microseconds timeElapsed);
	void UpdateView(std::chrono::microseconds timeElapsed);

	void AskUpdateContextMenu(uf::vec3i tile);
	void ClickContextMenu(uint8_t node, uint8_t verb);

	void SetPlayer(Player * const player);
	void TrackObject(Object *obj);

	void SetPosition(const Tile * const tile);
	const Tile *GetPosition() { return tile; }
	
	uf::vec3i Camera::ViewCoordsToWorldCoords(uf::vec3i viewCoords);

	void Suspend();
	bool IsSuspense() const { return suspense; }

	void SetFOV(int fov);
	void SetFOVZ(int fovZ);
	int GetFOV();
	int GetFOVZ();

	void SetInvisibleVisibility(uint visibility) { seeInvisibleAbility = visibility; }
	uint GetInvisibleVisibility() const { return seeInvisibleAbility; }

	void SetOverlay(uptr<ICameraOverlay> &&cameraOverlay);
	void ResetOverlay();

private:
	void updateOverlay(std::chrono::microseconds timeElapsed);
	void updateContextMenu();

	void fillEmptyVisibleBlocks();
	void fullRecountVisibleBlocks();
	void refreshVisibleBlocks();
	void updateFOV();

	void unseeTile(uf::vec3u pos);

	Player *player{};
	Object *trackingObject{};

	uint seeInvisibleAbility{};

	// View information
	const Tile *tile{};
	const Tile *lastTile{};
	uf::Grid<std::pair<Tile *, bool>> visibleTiles;
	std::unordered_set<uint> visibleObjects;

	int fov{};
	int fovZ{};

	bool suspense{};

	uptr<ICameraOverlay> overlay;

	// Update options
	bool changeFocus{};
	bool changeFov{};
	bool unsuspended{};
	bool cameraMoved{};

	bool askedUpdateContextMenu{};
	uf::vec3i contextMenuTileCoords;
	Tile *contextMenuTile;
	std::vector<ContextMenuNodeCache> contextMenuCache;

	int getVisibleAreaSide();
	int getVisibleAreaHeight();
	uf::vec3i getVisibleAreaDimensions();
	uf::vec3i getFirstTilePos();
};
