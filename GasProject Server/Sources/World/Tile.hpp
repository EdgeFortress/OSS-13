#pragma once

#include <list>

#include "Shared/Global.hpp"
#include "Shared/Types.hpp"

using std::list;

class Object;
class Block;
class Map;

struct TileInfo;

class Tile {
public:
    explicit Tile(Map *map, int x, int y);

    //void CheckLocal();
    void Update();

    // Removing object from tile content, but not deleting it, and change object.tile pointer
    // Also generate DeleteDiff
    bool RemoveObject(Object *obj);
    void MoveTo(Object *);
    void PlaceTo(Object *);

    int X() const { return x; }
    int Y() const { return y; }
    Block *GetBlock() const;
    Map *GetMap() const { return map; }

    bool IsDense();

    //void SetLocal(Local *local) { this->local = local; }

    //Test
    //const list<Object *> GetContentCopy() const { return content; };

    const TileInfo GetTileInfo(uint visibility) const;

private:
    Map *map;
    int x, y;
    Global::Sprite sprite;
    //Local *local;

    list<Object *> content;
    //list<Gas> listGas;

    // Add object to the tile, and change object.tile pointer
    // If object was in content of other tile, generate MoveDiff,
    // else generate AddDiff
    void addObject(Object *obj);
};