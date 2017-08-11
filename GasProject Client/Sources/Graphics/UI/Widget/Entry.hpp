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
    //sf::Vector2f size;

    sf::RectangleShape cursor;

    int characterSize;
    unsigned showPos;
    int cursorPos;
    sf::Time cursorTime;

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

	virtual void Update(sf::Time timeElapsed) final;
	virtual bool HandleEvent(sf::Event event) final;

	void Clear();
	void HideSymbols(wchar_t hider);
	void ShowSymbols();
    void SetOnEnterFunc(std::function<void()>);
	void SetActive(bool active);

	bool IsActive();
	std::string GetText();
};