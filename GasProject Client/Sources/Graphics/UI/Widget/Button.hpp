#pragma once

#include <functional>

#include "Widget.hpp"

class Button : public Widget {
private:
    sf::Color color;
    sf::Font font;
    sf::String name;
    sf::Text text;
    sf::Vector2f size;

    std::function<void()> func;

protected:
    virtual void draw() const final;

public:
    Button(sf::String &name, sf::Vector2f &size, const sf::Color &, const sf::Font &, std::function<void()> &);

    virtual void Update() final;
    virtual void HandleEvent(sf::Event event) final;
    
    void SetPosition(const sf::Vector2f);
    void SetPosition(const float x, const float y);
};