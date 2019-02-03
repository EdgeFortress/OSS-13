#pragma once

#include <string>
#include <list>
#include <vector>

#include "Shared/Types.hpp"

struct ObjectInfo {
    uint32_t id;
    std::string name;
    std::vector<uint32_t> sprites;
    uint layer;
	uf::Direction direction;
    bool dense;

    float moveSpeed;
    uf::vec2f constSpeed;

	ObjectInfo(uint32_t id, const std::vector<uint32_t> &sprites, const std::string &name, uint32_t layer, uf::Direction direction, bool dense = false) :
        id(id), sprites(sprites), name(name), layer(layer), direction(direction), dense(dense) { }
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
