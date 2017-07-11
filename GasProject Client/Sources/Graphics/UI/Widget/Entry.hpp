#pragma once

#include "Widget.hpp"

#include <vector>
#include <string>

class Entry : public Widget {
private:
    sf::Color color;
    sf::Font font;
    sf::Text text;
    sf::Vector2f size;

    sf::RectangleShape cursor;

    int characterSize;
    unsigned showPos;
    int cursorPos;
    sf::Time cursorTime;

    std::map < sf::Uint32, std::vector<float >> sizes; //0. Normal size, 1. Bold size, 2. Normal advance size, 3. Bold advance size;
    void SizeFiller(const sf::Font &font, sf::Uint32 c);

    std::wstring entryString;

    bool active;

protected:
    virtual void draw() const final;

public:
    Entry(sf::Vector2f &size, const sf::Color &, const sf::Font &);

    void SetSymbol(const wchar_t c);
    void DeleteSymbol();

    void MoveLeft();
    void MoveRight();

    virtual void Update(sf::Time timeElapsed) final;
    virtual void HandleEvent(sf::Event event) final;

    void SetPosition(const sf::Vector2f);
    void SetPosition(const float x, const float y);
};