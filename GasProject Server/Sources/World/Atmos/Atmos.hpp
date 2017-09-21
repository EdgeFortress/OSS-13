#pragma once

#include <list>

#include "Locale.hpp"
#include "Gases.hpp"
#include "Shared/Types.hpp"

class Map;
class Tile;

class Atmos {
public:
    explicit Atmos(Map *map);

    void Update(sf::Time timeElapsed);

    void CreateLocale(Tile *);
    void RemoveLocale(Locale *);

private:
    Map *map;
    std::list<uptr<Locale>> locales;
};
