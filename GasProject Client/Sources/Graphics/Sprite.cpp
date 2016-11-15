#include <string>

#include "Sprite.hpp"
#include "Client.hpp"
#include "Window.hpp"

#include <SFML/Graphics.hpp>

using std::string;

Texture::Texture(string path, int sizeOfTile, int numOfSprites) :
    texture(new sf::Texture),
    sizeOfTile(sizeOfTile),
    spritesInfo(numOfSprites)
{
    texture->loadFromFile(path);
    xNumOfTiles = texture->getSize().x / sizeOfTile;
    yNumOfTiles = texture->getSize().y / sizeOfTile;
}

Sprite *Texture::SetInfo(Global::Sprite key, int num, int firstFrame, bool directed, int frames) {
    spritesInfo[num].firstFrame = firstFrame;
    spritesInfo[num].directed = directed;
    spritesInfo[num].frames = frames;
    int firstFrameIndex = 0;
    for (int i = 0; i < num; i++) {
        if (spritesInfo[i].firstFrame == -1) {
            CC::log << "Wrong order of sprites info setting" << endl;
            break;
        }
        firstFrameIndex += spritesInfo[i].frames * (static_cast<int>(spritesInfo[i].directed) * 3 + 1);
    }
    return new Sprite(this, key, num, firstFrameIndex, frames, directed);
}

Sprite::Sprite() : sprite(new sf::Sprite()) {
    texture = nullptr;
    spriteIndex = frames = 0;
    curFrame = -1;
    scale = 1;
    directed = false;
    key = Global::Sprite::EMPTY;
}

Sprite::Sprite(Texture *t, Global::Sprite key, int spriteIndex, int firstFrameIndex, int frames, bool directed) :
    sprite(new sf::Sprite(*t->GetSFMLTexture())),
    texture(t),
    spriteIndex(spriteIndex), firstFrameIndex(firstFrameIndex),
    scale(1), directed(directed), frames(frames), curFrame(0), key(key)
{

}

void Sprite::Draw(sf::RenderWindow * const window, const int x, const int y, const int direction) const {
    if (this) {
        sf::Rect<int> rect;

        int realState = firstFrameIndex;
        if (directed) realState += direction * frames;
        if (frames > 1) realState += curFrame;

        rect.left = realState % texture->GetXNumOfTiles() * texture->GetSizeOfTile();
        rect.top = realState / texture->GetXNumOfTiles() * texture->GetSizeOfTile();
        rect.width = rect.height = texture->GetSizeOfTile();

        sprite->setPosition(static_cast<float>(x), static_cast<float>(y));
        sprite->setTextureRect(rect);
        sprite->setScale(scale, scale);
        window->draw(*sprite);
    }
}

void Sprite::Resize(int size) {
    scale = float(size) / texture->GetSizeOfTile();
}