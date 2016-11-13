#pragma once

#include <vector>
#include <list>

#include "Common/Useful.hpp"

class Tile;
class Block;
struct Diff;

namespace sf {
    class Packet;
}

class Camera {
private:
    const Tile *tile;
    bool suspense;

    std::vector< std::vector<Block *> > visibleBlocks;

    void countVisibleBlocks();

public:
    Camera();
    explicit Camera(const Tile * const tile);

    void SetPosition(const Tile * const tile);
    const Tile * const GetPosition() const { return tile; }
    void Suspend();

    const std::list<sptr<Diff>> GetVisibleDifferences();

    friend sf::Packet &operator<<(sf::Packet &, const Camera &);
};