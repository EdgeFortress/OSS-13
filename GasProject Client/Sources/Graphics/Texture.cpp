#include "Texture.hpp"

#include "Client.hpp"
#include "Sprite.hpp"

Texture::Texture(std::string path, nlohmann::json &config) :
    texture(new sf::Texture)
{
    texture->loadFromFile(path);

    sizeOfTile = config["tileSize"];

    xNumOfTiles = texture->getSize().x / sizeOfTile;
    yNumOfTiles = texture->getSize().y / sizeOfTile;

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

bool Texture::IsFramePixelTransparent(int x, int y, uint frame) const {
    const uint textureX = x + frame % xNumOfTiles * sizeOfTile;
    const uint textureY = y + frame / xNumOfTiles * sizeOfTile;

    if (textureX < 0 || textureX >= texture->getSize().x || textureY < 0 || textureY >= texture->getSize().y) return true;
    if (texture->copyToImage().getPixel(x, y).a == 0) return true;

    return false;
}

const std::vector<Sprite *> &Texture::GetSprites() const { return sprites; }
sf::Texture *Texture::GetSFMLTexture() const { return texture.get(); }

int Texture::GetXNumOfTiles() const { return xNumOfTiles; }
int Texture::GetYNumOfTiles() const { return yNumOfTiles; }
int Texture::GetSizeOfTile() const { return sizeOfTile; }

