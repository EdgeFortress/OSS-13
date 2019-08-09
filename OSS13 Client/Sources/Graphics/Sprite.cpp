#include "Sprite.hpp"

#include <string>

#include "Client.hpp"
#include "Window.hpp"

#include <SFML/Graphics.hpp>

using std::string;

Sprite::Sprite() :
    texture(nullptr),
	firstFrame(0),
	frames(1),
	directed(false),
	curFrame(0),
	direction(uf::Direction::NONE),
	scale(1)
{ }

void Sprite::Draw(sf::RenderTarget *target, uf::vec2i pos) const {
    sfSprite.setPosition(pos);
    target->draw(sfSprite);
}

void Sprite::Resize(int tileSize) {
    scale = float(tileSize) / texture->GetSizeOfTile();
    sfSprite.setScale(scale, scale);
}

bool Sprite::Update(sf::Time timeElapsed) {
    if (frames > 1) {
        curFrameTime += timeElapsed;
        if (curFrameTime >= frameTime) {
            curFrameTime = sf::Time();
            curFrame++;
            if (curFrame >= frames) {
                curFrame = 0;
                updateSpriteVariables();
                return true;
            }
            updateSpriteVariables();
        }
        return false;
    }
    return true;
}

void Sprite::SetDirection(uf::Direction direction) {
    this->direction = direction;
    updateSpriteVariables();
}

const std::string &Sprite::GetKey() const { return key; }
bool Sprite::IsValid() const { return texture; }
bool Sprite::IsAnimated() const { return frames > 1; }
bool Sprite::PixelTransparent(uf::vec2u pixel) const {
    auto real_pixel = pixel / scale;
    if (texture->IsFramePixelTransparent(real_pixel, curFrame + firstFrame)) return true;
    return false;
}

void Sprite::updateSpriteVariables() {
    if (!texture)
        return;

    int realState = firstFrame;
    if (directed && direction != uf::Direction::NONE) realState += int(direction) * frames;
    if (frames > 1) realState += curFrame;

    rect.left = realState % texture->GetNumOfTiles().x * texture->GetSizeOfTile();
    rect.top = realState / texture->GetNumOfTiles().x * texture->GetSizeOfTile();
    rect.width = rect.height = texture->GetSizeOfTile();

    sfSprite.setTexture(*texture->GetSFMLTexture());
    sfSprite.setTextureRect(rect);
}