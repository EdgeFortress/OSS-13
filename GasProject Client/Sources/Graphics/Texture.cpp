#include "Texture.hpp"

#include "Client.hpp"
#include "Sprite.hpp"

Texture::Texture(std::string path, nlohmann::json &config) :
    texture(new sf::Texture)
{
    texture->loadFromFile(path);

    sizeOfTile = config["tileSize"];
    numOfTiles = texture->getSize() / sizeOfTile;

    std::string title; // Can be same for few sequential sprites
    uint firstFrame = 0;
    for (auto sprite_config : config["sprites"]) {
        const auto titleIter = sprite_config.find("sprite");
        if (titleIter != sprite_config.end()) {
            title = titleIter->get<std::string>();
        }

        const auto framesIter = sprite_config.find("frames");
        int frames = 1;
        if (framesIter != sprite_config.end()) {
            frames = framesIter->get<int>();
        }

        const auto directedIter = sprite_config.find("directed");
        bool directed = false;
        if (directedIter != sprite_config.end()) {
            directed = directedIter->get<bool>();
        }

        SpriteInfo spriteInfo;
        spriteInfo.firstFrame = firstFrame;
        spriteInfo.directed = directed;
        spriteInfo.frames = frames;

        spritesInfo.push_back(std::move(spriteInfo));
        sprites.push_back(new Sprite(this, title, firstFrame, frames, directed));

        firstFrame += frames * (int(directed) * 3 + 1);
    }
}

bool Texture::IsFramePixelTransparent(uf::vec2u pixel, uint frame) const {
    uf::vec2u texturePixel = pixel;

    texturePixel.x += frame % numOfTiles.x * sizeOfTile;
    texturePixel.y += frame / numOfTiles.y * sizeOfTile;

    if (texturePixel >= uf::vec2i(0, 0) && texturePixel < texture->getSize()) {
        if (texture->copyToImage().getPixel(texturePixel.x, texturePixel.y).a == 0) return true;
    }

    return false;
}

const std::vector<Sprite *> &Texture::GetSprites() const { return sprites; }
sf::Texture *Texture::GetSFMLTexture() const { return texture.get(); }

uf::vec2i Texture::GetNumOfTiles() const { return numOfTiles; }
int Texture::GetSizeOfTile() const { return sizeOfTile; }

