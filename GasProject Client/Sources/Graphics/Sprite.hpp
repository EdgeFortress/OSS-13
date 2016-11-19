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

    bool PixelTransparent(const unsigned x, const unsigned y, 
                          const int sprite, 
                          const int direction, 
                          const int frame) const;

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

    void Draw(sf::RenderWindow * const window, const int x, const int y, const int direction) const;

    void Resize(int size);

    Global::Sprite GetKey() const { return key; }

    bool Animated() { return frames > 1; }
    void UpdateFrame() { ++curFrame %= frames; }

    bool PixelTransparent(const int x, const int y) const {
        if (texture->PixelTransparent(int(x / scale), int(y / scale), spriteIndex, directed, curFrame)) return true;
        return false;
    }
};