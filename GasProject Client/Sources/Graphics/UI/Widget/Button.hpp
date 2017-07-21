#pragma once

#include <functional>

#include "Widget.hpp"

class Button : public Widget {
private:
    sf::Color color, backColor;
    sf::Font font;
    sf::String name;
    sf::Text text;
    sf::Time diffBackColorTime;
    sf::Vector2f size;

    bool diffBackColor;

    std::function<void()> func;

    void updateCursorPointing(sf::Time);

protected:
    virtual void draw() const final;

public:
    Button(const sf::String &name, sf::Vector2f &size, const sf::Color &, const sf::Font &, std::function<void()>);

    virtual void Update(sf::Time timeElapsed) final;
    virtual void HandleEvent(sf::Event event) final;
    
    void SetPosition(const sf::Vector2f);
    void SetPosition(const float x, const float y);
};