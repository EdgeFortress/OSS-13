#pragma once

#include <unordered_map>

#include "Shared/Types.hpp"

class ResourceManager {
public:
    ResourceManager();

    uint GetIconNum(const std::string &);

private:
    std::unordered_map<std::string, uint> icons;
    std::unordered_map<std::string, uint> sounds;
};
