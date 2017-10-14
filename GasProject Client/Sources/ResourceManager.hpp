#pragma once

#include <vector>

#include "Graphics/Sprite.hpp"

class ResourceManager {
public:
    ResourceManager();

    Sprite GetSprite(uint id);

private:
    std::vector<uptr<Texture>> textures;
    std::vector<Texture *> spritesMap;
    //std::vector<uptr<Sounds>> sounds;

    uint nextSpriteNum;
};