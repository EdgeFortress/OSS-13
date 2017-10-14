#pragma once

#include <vector>
#include <string>
#include <SFML/Graphics.hpp>

#include "Shared/Types.hpp"
#include "Shared/JSON.hpp"

class Sprite;

class Texture {
public:
    Texture(std::string path, nlohmann::json &config, uint firstSpriteID);

    bool IsFramePixelTransparent(uf::vec2u pixel, uint frame) const;

    //const std::vector<Sprite *> &GetSprites() const;
    const sf::Texture *GetSFMLTexture() const;

    Sprite GetSprite(uint id) const;
    uf::vec2i GetNumOfTiles() const;
    uint GetNumOfSprites() const;
    uint GetSizeOfTile() const;

private:
    struct SpriteInfo {
        std::string title;
        uint firstFrame;
        uint frames;
        sf::Time frameTime;
        bool directed;
    };

    sf::Texture texture;
    uint sizeOfTile;
    uf::vec2u numOfTiles;
    uint firstSpriteID;
    std::vector<SpriteInfo> spritesInfo;
};