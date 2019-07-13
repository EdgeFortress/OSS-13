#pragma once

#include <list>
#include <vector>

#include <SFML/System.hpp>

#include <World/Atmos/Gases.hpp>
#include <Resources/IconInfo.h>

#include <Shared/Global.hpp>
#include <Shared/Types.hpp>

using std::list;
using std::vector;

class Object;
class Map;
class Locale;

struct Diff;
struct TileInfo;

class Tile {
public:
    friend Locale;
    Tile(Map *map, apos pos);

    void Update(std::chrono::microseconds timeElapsed);

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

	uf::vec3i GetPos() const;
    Map *GetMap() const;
    bool IsDense() const;
    bool IsSpace() const;
	Locale *GetLocale() const;

    const TileInfo GetTileInfo(uint viewerId, uint visibility) const;

    void AddDiff(Diff *diff);
    const list<sptr<Diff>> GetDifferences() const { return differences; }
    void ClearDiffs();

    int X() const { return pos.x; }
    int Y() const { return pos.y; }
    int Z() const { return pos.z; }

private:
    Map *map;
    uf::vec3i pos;
    IconInfo icon;

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

    list<sptr<Diff>> differences;

    // Add object to the tile, and change object.tile pointer
    // For moving use MoveTo, for placing PlaceTo
    void addObject(Object *obj);
    // Not generate Diff
    bool removeObject(Object *obj);
};