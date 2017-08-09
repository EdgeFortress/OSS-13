#pragma once

#include "Widget.hpp"

#include <functional>
#include <string>
#include <vector>

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

    //std::map < sf::Uint32, std::vector<float >> sizes; //((0. Normal size, 1. Bold size, ))0. Normal advance size, 1. Bold advance size;
    //void sizeFiller(const sf::Font &font, sf::Uint32 c);

    std::vector<float> getLetterSizes(wchar_t);
    void moveCursorRight(wchar_t);
    void moveCursorLeft(wchar_t);

    std::wstring entryString;

    bool active;
    bool hidingSymbols;
    
    wchar_t hidingSymbol;
    std::wstring hidingString;

    void setSymbol(wchar_t c);
    void deleteSymbol();

    void moveLeft();
    void moveRight();

    std::function<void()> onEnterFunc;

protected:
    virtual void draw() const final;

public:
    Entry(sf::Vector2f &size = sf::Vector2f());
    Entry(sf::Vector2f &size, const sf::Color &, const sf::Font &, bool hidingSymbols = false, wchar_t hidingSymbol = '*');

    void AddOnEnterFunc(std::function<void()>);

    void GrabFocus();
    void LoseFocus();
    bool HasFocus();

    void Clear();
    std::string GetText();
    void HideSymbols(wchar_t hider);
    void ShowSymbols();

    virtual void Update(sf::Time timeElapsed) final;
    virtual bool HandleEvent(sf::Event event) final;

    //void SetPosition(const sf::Vector2f);
    //void SetPosition(const float x, const float y);
};