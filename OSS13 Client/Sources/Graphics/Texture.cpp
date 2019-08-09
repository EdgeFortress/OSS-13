#include "Texture.hpp"

#include "Client.hpp"
#include "Sprite.hpp"

Texture::Texture() { }

bool Texture::IsFramePixelTransparent(uf::vec2u pixel, uint frame) const {
    uf::vec2u texturePixel = pixel;

    texturePixel.x += frame % numOfTiles.x * sizeOfTile;
    texturePixel.y += frame / numOfTiles.x * sizeOfTile;

    if (texturePixel >= uf::vec2i(0, 0) && texturePixel < texture.getSize()) {
        if (texture.copyToImage().getPixel(texturePixel.x, texturePixel.y).a == 0) return true;
        return false;
    }

    return true;
}

uf::vec2i Texture::GetNumOfTiles() const { return numOfTiles; }
uint Texture::GetSizeOfTile() const { return sizeOfTile; }
const sf::Texture *Texture::GetSFMLTexture() const { return &texture; }

bool Texture::load(std::wstring path, uint32_t tileSize) {
	if (!texture.loadFromFile(std::string(path.begin(), path.end())))
		return false;

	sizeOfTile = tileSize;
	numOfTiles = texture.getSize() / sizeOfTile;
	return true;
}