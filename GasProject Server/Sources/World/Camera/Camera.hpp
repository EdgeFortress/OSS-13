#pragma once

#include <vector>

#include <SFML/System/Time.hpp>

#include <Shared/Types.hpp>

#include "ICameraOverlay.h"

class Tile;
class Block;
class Mob;
class Player;
struct Diff;

namespace sf {
    class Packet;
}

class Camera {
public:
    explicit Camera(const Tile * const tile = nullptr);

    void UpdateView(sf::Time timeElapsed);

    void SetPlayer(Player * const player) { this->player = player; changeFocus = true; }
    void SetPosition(const Tile * const tile);
    void Suspend();
	void SetInvisibleVisibility(uint visibility) { seeInvisibleAbility = visibility; }

    bool IsSuspense() const { return suspense; }
	const Tile * const GetPosition() const { return tile; }
	uint GetInvisibleVisibility() const { return seeInvisibleAbility; }

private:
	void updateOverlay(sf::Time timeElapsed);

private:
	Player *player;

	uint seeInvisibleAbility;

	// View information
	const Tile *tile;
	const Tile *lasttile;
	uint visibleBlocksNum;
	int firstBlockX;
	int firstBlockY;
	std::vector<std::vector<Block *>> visibleBlocks;
	std::vector<std::vector<bool>> blocksSync;

	bool suspense;
	bool changeFocus;

	std::vector<uptr<ICameraOverlay>> overlays;

	// Update options
	bool blockShifted;
	bool unsuspensed;
	bool cameraMoved;

	void fullRecountVisibleBlocks(const Tile * const tile);
	void refreshVisibleBlocks(const Tile * const tile);
};