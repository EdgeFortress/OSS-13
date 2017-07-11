#include "Button.hpp"
#include "Client.hpp"

#include <iostream>

Button::Button(sf::String &name, sf::Vector2f &size, const sf::Color &color, const sf::Font &font, std::function<void()> &func)
    : Widget(size), name(name), size(size), color(color), font(font), func(func), text(name, font, 16) {
    text.setFillColor(color);
    text.setOutlineColor(color);
    SetSize(sf::Vector2f(text.getGlobalBounds().width + text.getGlobalBounds().left, text.getGlobalBounds().height + text.getGlobalBounds().top));
}

void Button::draw() const {
    sf::RenderTexture &buffer = const_cast<sf::RenderTexture &>(this->buffer);
    buffer.clear(sf::Color::Green);
    buffer.draw(text);
    buffer.display();
}

void Button::Update(sf::Time timeElapsed) {}

void Button::HandleEvent(sf::Event event) {
    int x = event.mouseButton.x,
        y = event.mouseButton.y;

    //if (event.type == sf::Event::MouseButtonPressed)
    //    std::cout << x << ' ' << GetAbsPosition().x << ' ' << GetAbsPosition().x + GetSize().x << ' ' << std::endl,
    //    std::cout << y << ' ' << GetAbsPosition().y << ' ' << GetAbsPosition().y + GetSize().y << ' ' << std::endl;

    if (event.type == sf::Event::MouseButtonPressed && x >= GetAbsPosition().x && x <= GetAbsPosition().x + GetSize().x && y >= GetAbsPosition().y && y <= GetAbsPosition().y + GetSize().y)
        func();
}

void Button::SetPosition(const sf::Vector2f pos) {
    //text.setPosition(pos);
    Widget::SetPosition(pos);
}
void Button::SetPosition(const float x, const float y) {
    //text.setPosition(x, y);
    Widget::SetPosition(x, y);
}