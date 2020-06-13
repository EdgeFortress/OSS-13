#pragma once

#include <vector>
#include <string>
#include <SFML/Graphics.hpp>

#include "Shared/Types.hpp"
#include "Shared/JSON.hpp"

class Sprite;
class TextureHolder;

class Texture {
friend TextureHolder;

public:
    Texture();

    bool IsFramePixelTransparent(uf::vec2u pixel, uint frame) const;

    const sf::Texture *GetSFMLTexture() const;
    uf::vec2i GetNumOfTiles() const;
    uint GetSizeOfTile() const;

protected:
	bool load(const std::string &path, uint32_t tileSize);

private:
    sf::Texture texture;
    uint sizeOfTile;
    uf::vec2u numOfTiles;
};
