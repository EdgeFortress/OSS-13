#pragma once

#include <vector>
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
	void SetFOV(int fov);
	void SetFOVZ(int fovZ);
	int GetFOV();
	int GetFOVZ();
    void Suspend();
	void SetInvisibleVisibility(uint visibility) { seeInvisibleAbility = visibility; }
	void SetOverlay(uptr<ICameraOverlay> &&cameraOverlay);
	void ResetOverlay();

    bool IsSuspense() const { return suspense; }
	const Tile * const GetPosition() const { return tile; }
	uint GetInvisibleVisibility() const { return seeInvisibleAbility; }

private:
	void updateOverlay(std::chrono::microseconds timeElapsed);
	void updateContextMenu();

private:
	Player *player{nullptr};
	Object *trackingObject{nullptr};

	uint seeInvisibleAbility{0};

	// View information
	const Tile *tile;
	const Tile *lasttile;
	int visibleTilesSide;
	int visibleTilesHeight;
	int firstBlockX;
	int firstBlockY;
	int firstBlockZ;
	uf::Grid<Tile *> visibleBlocks;
	uf::Grid<bool> blocksSync;
	std::unordered_set<uint> visibleObjects;

	int fov{0};
	int fovZ{0};

	int fovBuffer{0};
	int fovZBuffer{0};

	bool suspense;
	bool changeFocus;
	bool changeFov{false};

	uptr<ICameraOverlay> overlay;

	// Update options
	bool blockShifted;
	bool unsuspensed;
	bool cameraMoved;

	bool askedUpdateContextMenu{false};
	uf::vec3i contextMenuTileCoords;
	Tile *contextMenuTile;
	std::vector<ContextMenuNodeCache> contextMenuCache;

	void fillEmptyVisibleBlocks();
	void fullRecountVisibleBlocks(const Tile * const tile);
	void refreshVisibleBlocks(const Tile * const tile);

	void updateFOV();
	void unsee(uf::vec3u pos);
};
