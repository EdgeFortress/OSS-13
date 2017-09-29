#pragma once

#include <list>
#include <vector>
#include <SFML/System.hpp>

#include "Shared/Global.hpp"
#include "Shared/Types.hpp"
#include "Atmos/Gases.hpp"

using std::list;
using std::vector;

class Object;
class Block;
class Map;
class Locale;

struct TileInfo;

class Tile {
public:
    explicit Tile(Map *map, int x, int y);

    void Update(sf::Time timeElapsed);

    // Call it when atmos initialized or tile atmos properties changed (floor or wall status updated)
    void CheckLocale();

    // Removing object from tile content, but not deleting it, and change object.tile pointer
    // Also generate DeleteDiff
    bool RemoveObject(Object *obj);
    void MoveTo(Object *);
    void PlaceTo(Object *);

    int X() const { return x; }
    int Y() const { return y; }
    Block *GetBlock() const;
    Map *GetMap() const { return map; }
    bool IsDense() const;
    bool IsSpace() const;

    const TileInfo GetTileInfo(uint visibility) const;

    friend Locale;

private:
    Map *map;
    int x, y;
    uint sprite;

    list<Object *> content;
    bool hasFloor;
    // true if has wall
    bool fullBlocked;
    // for thin walls
    vector<bool> directionsBlocked;


    Locale *locale;
    bool needToUpdateLocale;
    // Partional pressures of gases by index
    vector<pressure> gases;
    pressure totalPressure;

    // Add object to the tile, and change object.tile pointer
    // For moving use MoveTo, for placing PlaceTo
    void addObject(Object *obj);
};