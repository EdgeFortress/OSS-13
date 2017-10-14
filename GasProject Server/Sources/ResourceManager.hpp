#pragma once

#include <unordered_map>

#include "Shared/Types.hpp"

class ResourceManager {
public:
    ResourceManager();

    uint GetSpriteNum(const std::string &);

private:
    std::unordered_map<std::string, uint> sprites;
    std::unordered_map<std::string, uint> sounds;
};
