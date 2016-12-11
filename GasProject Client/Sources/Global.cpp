#include "Common/NetworkConst.hpp"

#include <SFML/System.hpp>

Global::Direction Global::VectToDirection(sf::Vector2i vector) {
    if (vector.x > 0) {
        if (vector.y > 0)  return Global::Direction::SOUTH_EAST;
        if (vector.y == 0) return Global::Direction::EAST;
        if (vector.y < 0)  return Global::Direction::NORTH_EAST;
    }
    if (vector.x == 0) {
        if (vector.y > 0)  return Global::Direction::SOUTH;
        if (vector.y == 0) return Global::Direction::NONE;
        if (vector.y < 0)  return Global::Direction::NORTH;
    }
    if (vector.x < 0) {
        if (vector.y > 0)  return Global::Direction::SOUTH_WEST;
        if (vector.y == 0) return Global::Direction::WEST;
        if (vector.y < 0)  return Global::Direction::NORTH_WEST;
    }
    return Global::Direction::NONE;
}

sf::Vector2i Global::DirectionToVect(Global::Direction direction) {
    switch (direction) {
        case Global::Direction::SOUTH:
            return sf::Vector2i(0, 1);
        case Global::Direction::WEST:
            return sf::Vector2i(-1, 0);
        case Global::Direction::NORTH:
            return sf::Vector2i(0, -1);
        case Global::Direction::EAST:
            return sf::Vector2i(1, 0);
        case Global::Direction::SOUTH_WEST:
            return sf::Vector2i(-1, 1);
        case Global::Direction::NORTH_WEST:
            return sf::Vector2i(-1, -1);
        case Global::Direction::NORTH_EAST:
            return sf::Vector2i(1, -1);
        case Global::Direction::SOUTH_EAST:
            return sf::Vector2i(1, 1);
    }
    return sf::Vector2i(0, 0);
}