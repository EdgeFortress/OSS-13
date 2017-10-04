#pragma once

#include "Texture.hpp"
#include "Shared/Types.hpp"

class Sprite {
public:
    Sprite(Texture *t, const std::string &key, uint firstFrameIndex, uint frames, bool directed);

    void Draw(sf::RenderTarget *, uf::vec2i pos, uf::Direction direction) const;
    void Resize(int size);
    void UpdateFrame();

    const std::string &GetKey() const;
    bool IsAnimated() const;
    bool PixelTransparent(uf::vec2u pixel) const;

private:
    Texture *texture;
    uptr<sf::Sprite> sprite;
    std::string key;
    uint firstFrame;
    uint frames;
    bool directed;

    uint curFrame;
    float scale;
};