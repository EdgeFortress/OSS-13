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

    bool IsFramePixelTransparent(uf::vec2u pixel, uint frame) const;

    const std::vector<Sprite *> &GetSprites() const;
    sf::Texture *GetSFMLTexture() const;

    uf::vec2i GetNumOfTiles() const;
    int GetSizeOfTile() const;

private:
    struct SpriteInfo {
        uint firstFrame;
        uint frames;
        bool directed;
    };

    uptr<sf::Texture> texture;
    uint sizeOfTile;
    uf::vec2u numOfTiles;
    std::vector<SpriteInfo> spritesInfo;
    std::vector<Sprite *> sprites;
};