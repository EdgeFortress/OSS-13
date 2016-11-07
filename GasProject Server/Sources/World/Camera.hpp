#pragma once

#include <vector>

class Tile;
class Block;

namespace sf {
    class Packet;
}

class Camera {
private:
    Tile *tile;
    bool suspense;

    std::vector< std::vector<Block *> > visibleBlocks;

    void countVisibleBlocks();

public:
    Camera();
    explicit Camera(Tile *tile);

    void SetPosition(Tile *tile);
    void Suspend();

    friend sf::Packet &operator<<(sf::Packet &, Camera &);
};