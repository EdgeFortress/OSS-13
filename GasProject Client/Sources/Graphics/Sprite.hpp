#pragma once

#include "Texture.hpp"
#include "Shared/Types.hpp"

class SpriteFactory;

class Sprite {
public:
    friend SpriteFactory;

    Sprite();
    Sprite(const Sprite &) = default;
    Sprite &operator=(const Sprite &) = default;
    Sprite(Sprite &&) = default;
    Sprite &operator=(Sprite &&) = default;

    void Draw(sf::RenderTarget *, uf::vec2i pos) const;
    // true if frame changed to the first
    bool Update(sf::Time timeElapsed);
    void Resize(int tileSize);
    
    void SetDirection(uf::Direction direction);

    const std::string &GetKey() const;
    // true if sprite has few frames
    bool IsValid() const;
    bool IsAnimated() const;
    bool PixelTransparent(uf::vec2u pixel) const;

private:
    mutable sf::Sprite sfSprite;

    const Texture *texture;
    std::string key;
    uint firstFrame;
    uint frames;
    sf::Time frameTime;
    bool directed;

    uint curFrame;
    uf::Direction direction;
    float scale;
    sf::Rect<int> rect;
    sf::Time curFrameTime;

    void updateSpriteVariables();
};
