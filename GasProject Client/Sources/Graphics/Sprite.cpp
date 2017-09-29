#include "Sprite.hpp"

#include <string>

#include "Client.hpp"
#include "Window.hpp"

#include <SFML/Graphics.hpp>

using std::string;

Sprite::Sprite(Texture *t, const std::string &key, uint firstFrame, uint frames, bool directed) :
    texture(t), sprite(new sf::Sprite(*t->GetSFMLTexture())), 
    key(key), firstFrame(firstFrame), frames(frames), directed(directed), 
    curFrame(0), scale(1)
{

}

void Sprite::Draw(sf::RenderTarget *target, int x, int y, const uf::Direction direction) const {
    sf::Rect<int> rect;

    int realState = firstFrame;
    if (directed && direction != uf::Direction::NONE) realState += int(direction) * frames;
    if (frames > 1) realState += curFrame;

    rect.left = realState % texture->GetXNumOfTiles() * texture->GetSizeOfTile();
    rect.top = realState / texture->GetXNumOfTiles() * texture->GetSizeOfTile();
    rect.width = rect.height = texture->GetSizeOfTile();

    sprite->setPosition(float(x), float(y));
    sprite->setTextureRect(rect);
    sprite->setScale(scale, scale);
    target->draw(*sprite);
}

void Sprite::Resize(int size) {
    scale = float(size) / texture->GetSizeOfTile();
}

void Sprite::UpdateFrame() {
    if (frames > 1) {
        curFrame++;
        if (curFrame >= frames)
            curFrame = 0;
    }
}

const std::string &Sprite::GetKey() const { return key; }
bool Sprite::IsAnimated() const { return frames > 1; }
bool Sprite::PixelTransparent(const int x, const int y) const {
    if (texture->IsFramePixelTransparent(uint(x / scale), uint(y / scale), curFrame)) return true;
    return false;
}
