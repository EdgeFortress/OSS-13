#pragma once

#include <vector>
#include <list>

#include "Gases.hpp"

class Tile;

class Locale {
public:
    Locale();

private:
    std::list<pressure> gases;
    std::list<Tile *> tiles;
};
