#include "Button.hpp"
#include "Client.hpp"
#include "Graphics/Window.hpp"

#include <iostream>

Button::Button(const sf::String &name, sf::Vector2f &size, const sf::Color &color, const sf::Font &font, std::function<void()> func)
    : Widget(size), name(name), size(size), color(color), font(font), func(func), text(name, font, 16) {
    text.setFillColor(color);
    text.setOutlineColor(color);
    //SetSize(sf::Vector2f(text.getGlobalBounds().width + text.getGlobalBounds().left, text.getGlobalBounds().height + text.getGlobalBounds().top));
    backColor = sf::Color::Transparent;
}

void Button::draw() const {
    sf::RenderTexture &buffer = const_cast<sf::RenderTexture &>(this->buffer);
    buffer.clear(backColor);
    buffer.draw(text);
    buffer.display();
}

void Button::Update(sf::Time timeElapsed) {
    if (diffBackColor) {
        diffBackColorTime += timeElapsed;

        if (diffBackColorTime > sf::seconds(0.3f)) {
            int x = sf::Mouse::getPosition().x - CC::Get()->GetWindow()->GetPosition().x - 9,
                y = sf::Mouse::getPosition().y - CC::Get()->GetWindow()->GetPosition().y - 38;
            //std::cout << x << ' ' << y << ' ' << GetAbsPosition().x << ' ' << GetAbsPosition().y << ' ' << GetAbsPosition().x + GetSize().x << ' ' << GetAbsPosition().y + GetSize().y << std::endl;
            if (x >= GetAbsPosition().x && x <= GetAbsPosition().x + GetSize().x &&
                y >= GetAbsPosition().y && y <= GetAbsPosition().y + GetSize().y)
                diffBackColorTime = sf::Time::Zero;
            else {
                diffBackColor = false;
                backColor = sf::Color::Transparent;
            }
        }
    }
}

void Button::HandleEvent(sf::Event event) {
    int buttonX = event.mouseButton.x,
        buttonY = event.mouseButton.y,
        mouseX = event.mouseMove.x,
        mouseY = event.mouseMove.y;

    //if (event.type == sf::Event::MouseButtonPressed)
    //    std::cout << x << ' ' << GetAbsPosition().x << ' ' << GetAbsPosition().x + GetSize().x << ' ' << std::endl,
    //    std::cout << y << ' ' << GetAbsPosition().y << ' ' << GetAbsPosition().y + GetSize().y << ' ' << std::endl;

    if (event.type == sf::Event::MouseButtonPressed && 
        buttonX >= GetAbsPosition().x && buttonX <= GetAbsPosition().x + GetSize().x && 
        buttonY >= GetAbsPosition().y && buttonY <= GetAbsPosition().y + GetSize().y)
        func();
    if (event.type == sf::Event::MouseMoved &&
        mouseX >= GetAbsPosition().x && mouseX <= GetAbsPosition().x + GetSize().x &&
        mouseY >= GetAbsPosition().y && mouseY <= GetAbsPosition().y + GetSize().y) {
        backColor = sf::Color::Magenta;
        diffBackColorTime = sf::Time::Zero;
        diffBackColor = true;
    }
}

void Button::SetPosition(const sf::Vector2f pos) {    
    Widget::SetPosition(pos);
    //text.setPosition(pos.x + GetSize().x / 2, pos.y + GetSize().y / 2);
}
void Button::SetPosition(const float x, const float y) {
    Widget::SetPosition(x, y);
    //text.setPosition(x + GetSize().x / 2, y + GetSize().y / 2);
}