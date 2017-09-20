#pragma once

#include <list>

#include "Locale.hpp"
#include "Gases.hpp"

class Map;

class Atmos {
public:
    explicit Atmos(Map *map);

private:
    Map *map;
    std::list<Locale> locales;

    void generateLocales();
};
