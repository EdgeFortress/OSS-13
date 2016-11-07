#pragma once

#include <vector>
#include <string>

//#include <SFML/Graphics.hpp>

#include "Common/Useful.hpp"
#include "Common/NetworkConst.hpp"

using std::vector;
using std::string;

class Sprite;

namespace sf {
    class RenderWindow;
    class Texture;
    class Sprite;
}

struct TextureSpriteInfo {
    int firstFrame;
    int frames;
    bool directed;

    TextureSpriteInfo() { firstFrame = -1; frames = 0; directed = false; }
};

class Texture {
private:
    uptr<sf::Texture> texture;
    int sizeOfTile;
    int xNumOfTiles, yNumOfTiles;
    vector<TextureSpriteInfo> spritesInfo;

public:
    Texture(string path, int sizeOfTile, int numOfSprites);

    Sprite *SetInfo(Global::Sprite, int num, int firstFrame, bool directed, int frames);

    /*bool PixelTransparent(int x, int y, int sprite, int direction, int frame) const {
        int realState = spritesInfo[sprite].firstFrame;
        if (spritesInfo[sprite].directed > 0) realState += direction * spritesInfo[sprite].frames;
        if (spritesInfo[sprite].frames > 1) realState += frame;

        x += realState % xNumOfTiles * sizeOfTile;
        y += realState / xNumOfTiles * sizeOfTile;

        if (x < 0 || x >= texture->getSize().x || y < 0 || y >= texture->getSize().y) return true;
        if (texture->copyToImage().getPixel(x, y).a == 0) return true;

        return false;
    }*/

    sf::Texture *GetSFMLTexture() const { return texture.get(); }

    int GetXNumOfTiles() const { return xNumOfTiles; }
    int GetYNumOfTiles() const { return yNumOfTiles; }
    int GetSizeOfTile() const { return sizeOfTile; }
};

class Sprite
{
private:
    Texture *texture;
    uptr<sf::Sprite> sprite;
    Global::Sprite key;
    int spriteIndex;
    int firstFrameIndex;
    float scale;
    int frames;
    bool directed;

    int curFrame;

public:
    Sprite();
    Sprite(Texture *t, Global::Sprite key, int spriteIndex, int firstFrameIndex, int frames, bool directed);

    void Draw(sf::RenderWindow *window, int x, int y, int direction) const;

    /*bool PixelTransparent(int x, int y) const
    {
        x /= scale; y /= scale;
        if (texture->PixelTransparent(x, y, num, directed, animated)) return true;
        return false;
    }*/
};