#pragma once

#include <vector>

#include "Graphics/Sprite.hpp"

class ResourceManager {
public:
    ResourceManager();

    void SpritesNextFrame();
    void SpritesResize(uint size);

    Sprite *GetSprite(uint id);

private:
    std::vector<uptr<Texture>> textures;
    std::vector<uptr<Sprite>> sprites;
    //std::vector<uptr<Sounds>> sounds;
};