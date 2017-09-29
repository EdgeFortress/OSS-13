#pragma once

#include <vector>
#include <string>
#include <SFML/Graphics.hpp>

#include "Shared/Types.hpp"
#include "Shared/JSON.hpp"

class Sprite;

class Texture {
public:
    Texture(std::string path, nlohmann::json &config);

    bool IsFramePixelTransparent(int x, int y, uint frame) const;

    const std::vector<Sprite *> &GetSprites() const;
    sf::Texture *GetSFMLTexture() const;

    int GetXNumOfTiles() const;
    int GetYNumOfTiles() const;
    int GetSizeOfTile() const;

private:
    struct SpriteInfo {
        uint firstFrame;
        uint frames;
        bool directed;

        SpriteInfo() { firstFrame = -1; frames = 0; directed = false; }
    };

    uptr<sf::Texture> texture;
    uint sizeOfTile;
    uint xNumOfTiles, yNumOfTiles;
    std::vector<SpriteInfo> spritesInfo;
    std::vector<Sprite *> sprites;
};