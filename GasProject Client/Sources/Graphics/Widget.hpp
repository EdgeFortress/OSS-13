#pragma once

#include <SFML/Graphics.hpp>

class Widget : public sf::Transformable, public sf::Drawable {
private:
    virtual void draw(sf::RenderTarget &target, sf::RenderStates states) const = 0;

public:
    virtual void setPosition(float x, float y) = 0;
    virtual void setScale(float factorX, float factorY) = 0;
    
    virtual void setSize(const sf::Vector2f &size) = 0;

    virtual void update() = 0;
};