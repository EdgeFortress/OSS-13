#pragma once

#include "Widget.hpp"

#include <functional>
#include <string>
#include <vector>

class Entry : public Widget {
public:
    Entry(const sf::Vector2f &size = sf::Vector2f());

    virtual void Update(sf::Time timeElapsed) final;
    virtual bool HandleEvent(sf::Event event) final;

    void Clear();
    void HideSymbols(wchar_t hider = '*');
    void ShowSymbols();
    void SetOnEnterFunc(std::function<void()>);

    std::string GetText();

protected:
    virtual void draw() const final;

private:
    sf::Text text;
    sf::RectangleShape cursor;

    unsigned showPos;
    int cursorPos;
    sf::Time cursorTime;

    std::vector<float> getLetterSizes(wchar_t);
    void moveCursorRight(wchar_t);
    void moveCursorLeft(wchar_t);

    std::wstring entryString;

    bool hidingSymbols;
    
    wchar_t hidingSymbol;
    std::wstring hidingString;

    void setSymbol(wchar_t c);
    void deleteSymbol();

    void moveLeft();
    void moveRight();

    std::function<void()> onEnterFunc;
};