#pragma once

#include <SFML/System.hpp>

namespace Global {
    const int PORT = 55678;

    const int BLOCK_SIZE = 8;
    const int FOV = 15; // Field Of View
    const int MIN_PADDING = 3;

    enum class DiffType : char {
        NONE = 0,
        ADD,
        REMOVE,
        RELOCATE,
        MOVE,
        SHIFT
    };

    enum class Sprite : char {
        EMPTY = 0,
        Floor,
        Wall,
        Airlock,
        Mob,
        Space,
        NextSprite = Space + 26
    };

    enum class Direction : char {
        NONE = -1,
        SOUTH,
        WEST,
        NORTH,
        EAST,
        SOUTH_WEST,
        NORTH_WEST,
        NORTH_EAST,
        SOUTH_EAST
    };

    Direction VectToDirection(sf::Vector2i);
    sf::Vector2i DirectionToVect(Direction);
}