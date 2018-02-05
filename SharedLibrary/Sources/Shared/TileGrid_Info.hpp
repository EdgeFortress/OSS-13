#pragma once

#include <string>
#include <list>

#include "Shared/Types.hpp"

struct ObjectInfo {
    const uint id;
    const std::string name;
    const uint sprite;
    const uint layer;
	const uf::Direction direction;
    const bool dense;

    float moveSpeed;
    uf::vec2f constSpeed;

	ObjectInfo(uint id, uint sprite, const std::string &name, const uint layer, const uf::Direction direction, const bool dense = false) :
        id(id), sprite(sprite), name(name), layer(layer), direction(direction), dense(dense) { }
};

struct TileInfo {
    const uint sprite;
    const std::list<ObjectInfo> content;

    TileInfo(uint sprite, const std::list<ObjectInfo> &content) :
        sprite(sprite), content(content) { }
    TileInfo() : 
        sprite(0), content() { }
};

struct BlockInfo {
    const int x, y;
    const std::list<TileInfo> tiles;

    BlockInfo(const int x, const int y, const std::list<TileInfo> &tiles) :
        x(x), y(y), tiles(tiles) { }
};
