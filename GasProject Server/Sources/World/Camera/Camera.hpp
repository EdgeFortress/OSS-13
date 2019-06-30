#pragma once

#include <vector>
#include <unordered_set>

#include <Shared/Types.hpp>

#include "ICameraOverlay.h"

class Tile;
class Mob;
class Player;
struct Diff;

namespace sf {
    class Packet;
}

class Camera {
public:
    explicit Camera(const Tile * const tile = nullptr);

    void UpdateView(std::chrono::microseconds timeElapsed);

    void SetPlayer(Player * const player) { this->player = player; changeFocus = true; }
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

	uint seeInvisibleAbility{0};

	// View information
	const Tile *tile;
	const Tile *lasttile;
	uint visibleTilesSide;
	uint visibleTilesHeight;
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

	uint flat_index (const apos c) const;
};