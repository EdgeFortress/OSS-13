#pragma once

#include <string>
#include <list>

struct ObjectInfo {
    const int sprite;
    const std::string name;

    ObjectInfo(const int sprite, const std::string &name) :
        sprite(sprite), name(name) { }
};

struct TileInfo {
    const int sprite;
    const std::list<ObjectInfo> content;

    TileInfo(const int sprite, const std::list<ObjectInfo> &content) :
        sprite(sprite), content(content) { }
    TileInfo() : 
        sprite(-1), content() { }
};

struct BlockInfo {
    const int x, y;
    const std::list<TileInfo> tiles;

    BlockInfo(const int x, const int y, const std::list<TileInfo> &tiles) :
        x(x), y(y), tiles(tiles) { }
};
