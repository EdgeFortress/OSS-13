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
    friend Locale;
    Tile(Map *map, uf::vec2i pos);

    void Update(sf::Time timeElapsed);

    // Call it when atmos initialized or tile atmos properties changed (floor or wall status updated)
    void CheckLocale();

    // Removing object from tile content, but not deleting it, and change object.tile pointer
    // Also generate DeleteDiff
    bool RemoveObject(Object *obj);
	// Smooth moving from one tile to another
    bool MoveTo(Object *);
	// Teleport or add to tile from nowhere
    void PlaceTo(Object *);

    const list<Object *> &Content() const;
    Object *GetDenseObject() const;

    uf::vec2i GetPos() const;
    Block *GetBlock() const;
    Map *GetMap() const;
    bool IsDense() const;
    bool IsSpace() const;

    const TileInfo GetTileInfo(uint visibility) const;

private:
    Map *map;
    uf::vec2i pos;
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
    // Not generate Diff
    bool removeObject(Object *obj);
};