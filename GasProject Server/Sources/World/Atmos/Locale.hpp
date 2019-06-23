#pragma once

#include <vector>
#include <list>
#include <chrono>

#include <Shared/Types.hpp>
#include <Shared/IFaces/IHasRepeatableID.h>

#include "Gases.hpp"

class Tile;
class Atmos;

class Locale : public IHasRepeatableID {
public:
    explicit Locale(Atmos *, Tile *);

    void Update(std::chrono::microseconds timeElapsed);

    // Add Tile to locale
    void AddTile(Tile *tile);
    // Remove Tile from Locale
    void RemoveTile(Tile *tile);
    // Merge with other locale
    void Merge(Locale *locale);
    // Remove all tiles
    void Clear();

    // Call if neighbour tile became space
    void Open();
    // Call if wall was build at near tile
    void CheckCloseness();

    bool IsEmpty() const;
    bool IsClosed() const;
    uint NumOfTiles() const;

    friend Atmos;

private:
    Atmos *atmos;
    
    std::list<pressure> gases;
    std::list<Tile *> tiles;

    // Status
    bool closed;
    bool needToCheckCloseness;
};
