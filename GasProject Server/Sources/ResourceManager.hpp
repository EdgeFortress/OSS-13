#pragma once

#include <unordered_map>

#include <Shared/Global.hpp>

class ResourceManager {
public:
    ResourceManager();

    uint32_t GetSpriteNum(const std::string &spriteName, Global::ItemSpriteState spriteState = Global::ItemSpriteState::DEFAULT);

private:
    std::unordered_map<std::string, uint32_t> sprites;
    std::unordered_map<std::string, uint32_t> sounds;
};
