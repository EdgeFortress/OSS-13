#pragma once

#include <vector>
#include <unordered_set>

#include <Shared/Types.hpp>

#include "ICameraOverlay.h"

class Tile;
class Object;
class Player;
struct Diff;

namespace sf {
    class Packet;
}

class Camera {
public:
    explicit Camera(const Tile * const tile = nullptr);

	void Update(std::chrono::microseconds timeElapsed);
    void UpdateView(std::chrono::microseconds timeElapsed);

    void SetPlayer(Player * const player);
	void TrackObject(Object *obj);
    void SetPosition(const Tile * const tile);
    void Suspend();
	void SetInvisibleVisibility(uint visibility) { seeInvisibleAbility = visibility; }
	void SetOverlay(uptr<ICameraOverlay> &&cameraOverlay);
	void ResetOverlay();

    bool IsSuspense() const { return suspense; }
	const Tile * const GetPosition() const { return tile; }
	uint GetInvisibleVisibility() const { return seeInvisibleAbility; }

private:
	void updateOverlay(std::chrono::microseconds timeElapsed);

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
	std::vector<Tile *> visibleBlocks;
	std::vector<bool> blocksSync;
	std::unordered_set<uint> visibleObjects;

	bool suspense;
	bool changeFocus;

	uptr<ICameraOverlay> overlay;

	// Update options
	bool blockShifted;
	bool unsuspensed;
	bool cameraMoved;

	void fullRecountVisibleBlocks(const Tile * const tile);
	void refreshVisibleBlocks(const Tile * const tile);

	int flat_index (uf::vec3i c) const;
};