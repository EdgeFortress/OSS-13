#pragma once

#include <string>
#include <vector>

#include <Shared/Types.hpp>

struct ObjectInfo {
    uint32_t id;
    std::string name;
    std::vector<uint32_t> spriteIds;
    uint layer;
	uf::Direction direction;
    bool dense;

    float moveSpeed;
    uf::vec2f constSpeed;
};

struct TileInfo {
	int x, y, z;
    uint sprite;
    std::vector<ObjectInfo> content;
};
