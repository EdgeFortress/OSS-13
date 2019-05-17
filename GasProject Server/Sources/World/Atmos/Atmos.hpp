#pragma once

#include <list>

#include <VerbsHolder.h>
#include <Shared/Types.hpp>

#include "Locale.hpp"
#include "Gases.hpp"

class Map;
class Tile;

class Atmos : public VerbsHolder {
public:
    explicit Atmos(Map *map);

    void Update(sf::Time timeElapsed);

    void CreateLocale(Tile *);
    void RemoveLocale(Locale *);

private:
    Map *map;
    std::list<uptr<Locale>> locales;
};
