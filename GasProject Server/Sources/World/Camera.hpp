#pragma once

#include <vector>
#include <list>

#include "Common/Useful.hpp"

class Tile;
class Block;
class Mob;
class Player;
struct Diff;

namespace sf {
    class Packet;
}

class Camera {
private:
    const Tile *tile;
    const Tile *lasttile;
    Player *player;
    //int relX, relY;
    bool suspense;
    bool changeFocus;

    int visibleBlocksNum;

    int firstBlockX;
    int firstBlockY;

    // Update options
    bool blockShifted;
    bool unsuspensed;
    bool cameraMoved;

    std::vector< std::vector<Block *> > visibleBlocks;
    std::vector< std::vector<bool> > blocksSync;

    void fullRecountVisibleBlocks(const Tile * const tile);
    void refreshVisibleBlocks(const Tile * const tile);

public:
    explicit Camera(const Tile * const tile = nullptr);

    void UpdateView();

    void SetPlayer(Player * const player) { this->player = player; changeFocus = true; }
    void SetPosition(const Tile * const tile);
    const Tile * const GetPosition() const { return tile; }
    void Suspend();

    bool IsSuspense() const { return suspense; }
};