#pragma once

namespace Global {
    const int PORT = 55678;

    const int BLOCK_SIZE = 8;
    const int FOV = 15; // Field Of View
    const int MIN_PADDING = 3;

    enum class Sprite : char {
        EMPTY = 0,
        Floor,
        Wall,
        Airlock,
        Mob,
        Space,
        NextSprite = Space + 26,
    };
}