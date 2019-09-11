#pragma once

#include <list>
#include <vector>

#include <SFML/System.hpp>

#include <World/Atmos/Gases.hpp>
#include <Resources/IconInfo.h>

#include <Shared/Global.hpp>
#include <Shared/Types.hpp>
#include <Shared/Geometry/DirectionSet.h>
#include <Shared/Network/Protocol/ServerToClient/WorldInfo.h>
#include <Shared/Network/Protocol/ServerToClient/Diff.h>

class Object;
class Map;
class Locale;

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

    const std::list<Object *> &Content() const;
    Object *GetDenseObject(uf::DirectionSet directions) const;

	uf::vec3i GetPos() const;
    Map *GetMap() const;
	bool IsDense(uf::DirectionSet directions) const;
    bool IsSpace() const;
	Locale *GetLocale() const;

	network::protocol::TileInfo GetTileInfo(uint viewerId, uint visibility) const;

	void AddDiff(std::shared_ptr<network::protocol::Diff> diff, Object *object);
    const std::vector<std::pair<std::shared_ptr<network::protocol::Diff>, sptr<Object>>> &GetDifferencesWithObject() const { return differencesWithObject; }
    void ClearDiffs();

    int X() const { return pos.x; }
    int Y() const { return pos.y; }
    int Z() const { return pos.z; }

private:
    Map *map;
    uf::vec3i pos;
    IconInfo icon;

    std::list<Object *> content;
    bool hasFloor;
    // true if has wall
    bool fullBlocked;
    // for thin walls
    std::vector<bool> directionsBlocked;


    Locale *locale;
    bool needToUpdateLocale;
    // Partional pressures of gases by index
    std::vector<pressure> gases;
    pressure totalPressure;

    std::vector<std::pair<std::shared_ptr<network::protocol::Diff>, sptr<Object>>> differencesWithObject;

    // Add object to the tile, and change object.tile pointer
    // For moving use MoveTo, for placing PlaceTo
    void addObject(Object *obj);
    // Not generate Diff
    bool removeObject(Object *obj);
};
