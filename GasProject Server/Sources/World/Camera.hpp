#pragma once

#include <vector>

class Tile;
class Block;

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

    friend sf::Packet &operator<<(sf::Packet &, Camera &);
};