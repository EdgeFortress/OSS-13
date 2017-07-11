#include "Entry.hpp"

#include <iostream>

using std::vector;
using std::wstring;

Entry::Entry(sf::Vector2f &size, const sf::Color &color, const sf::Font &font)
    : Widget(size), size(size), color(color), font(font), text("", font, 16) {

    //std::cout << "Filling sizes" << std::endl;

    //for (sf::Uint32 c = L'a'; c <= L'z'; c++)
    //    SizeFiller(font, c);
    //for (sf::Uint32 c = L'A'; c <= L'Z'; c++)
    //    SizeFiller(font, c);

    //for (auto i : sizes)
    //    std::cout << i.second[0] << ' ';

    active = true;

    showPos = 0;
    cursorPos = -1;

    characterSize = text.getCharacterSize();

    cursorTime = sf::Time::Zero;
    cursor.setSize(sf::Vector2f(characterSize / 8.0f, float(characterSize)));
    cursor.setFillColor(color);

    text.setFillColor(color);
    text.setOutlineColor(color);
}

void Entry::SizeFiller(const sf::Font &font, sf::Uint32 c) {
    sizes[c].push_back(font.getGlyph(c, characterSize, false).bounds.width);
    sizes[c].push_back(font.getGlyph(c, characterSize, true).bounds.width);
    sizes[c].push_back(font.getGlyph(c, characterSize, false).advance);
    sizes[c].push_back(font.getGlyph(c, characterSize, true).advance);
}

void Entry::draw() const {
    sf::RenderTexture &buffer = const_cast<sf::RenderTexture &>(this->buffer);
    buffer.clear(sf::Color::White);
    buffer.draw(text);

    if (active && cursorTime >= sf::seconds(0.6f) && cursorTime <= sf::seconds(0.8f))
        buffer.draw(cursor);

    buffer.display();
}

void Entry::Update(sf::Time timeElapsed) {
    cursorTime += timeElapsed;

    if (cursorTime > sf::seconds(0.8f))
        cursorTime = sf::Time::Zero;
}

void Entry::SetSymbol(const wchar_t c) {
    if (cursorPos == entryString.size() - 1)
        entryString += c;
    else
        entryString.insert(entryString.begin() + 1 + cursorPos, c);

    cursorPos += 1;

    vector<float> letter_sizes = std::move(sizes[c]);
    if (letter_sizes.size())
        cursor.setPosition(cursor.getPosition().x + letter_sizes[2], cursor.getPosition().y);
    else {
        //std::cout << "Loading" << std::endl;
        SizeFiller(font, c);
        letter_sizes = std::move(sizes[c]);
        cursor.setPosition(cursor.getPosition().x + letter_sizes[2], cursor.getPosition().y);
    }

    while (cursor.getPosition().x >= text.getGlobalBounds().left + size.x * 0.96f) {
        showPos++;
        text.setString(wstring(entryString.c_str() + showPos));
        letter_sizes = std::move(sizes[entryString[showPos - 1]]);
        if (letter_sizes.size())
            cursor.setPosition(cursor.getPosition().x - letter_sizes[2], cursor.getPosition().y);
        else {
            SizeFiller(font, entryString[showPos - 1]);
            letter_sizes = std::move(sizes[entryString[showPos - 1]]);
            cursor.setPosition(cursor.getPosition().x - letter_sizes[2], cursor.getPosition().y);
        }
    }

    text.setString(wstring(entryString.c_str() + showPos));
}

void Entry::DeleteSymbol() {
    if (cursorPos < 0)
        return;

    vector<float> letter_sizes = std::move(sizes[entryString[cursorPos]]);
    if (letter_sizes.size()) {
        cursor.setPosition(cursor.getPosition().x - letter_sizes[2], cursor.getPosition().y);
    }
    else {
        SizeFiller(font, entryString[cursorPos]);
        letter_sizes = std::move(sizes[entryString[cursorPos]]);
        cursor.setPosition(cursor.getPosition().x - letter_sizes[2], cursor.getPosition().y);
    }

    entryString.erase(entryString.begin() + cursorPos);
    text.setString(wstring(entryString.c_str() + showPos));

    cursorPos--;

    if (cursorPos < 0)
        return;

    int shift_size = 15;
    if (cursorPos + 1 <= int(showPos) && showPos) {
        if (int(showPos) >= shift_size)
            showPos -= shift_size;
        else {
            shift_size = showPos;
            showPos -= shift_size;
        }
        for (unsigned i = showPos; i < showPos + shift_size; i++) {
            letter_sizes = std::move(sizes[entryString[i]]);
            if (letter_sizes.size())
                cursor.setPosition(cursor.getPosition().x + letter_sizes[2], cursor.getPosition().y);
            else {
                SizeFiller(font, entryString[i]);
                letter_sizes = std::move(sizes[entryString[i]]);
                cursor.setPosition(cursor.getPosition().x + letter_sizes[2], cursor.getPosition().y);
            }
        }
    }

    text.setString(wstring(entryString.c_str() + showPos));
}

void Entry::MoveLeft() {
    if (cursorPos < 0)
        return;
    vector<float> letter_sizes = std::move(sizes[entryString[cursorPos]]);
    if (letter_sizes.size())
        cursor.setPosition(cursor.getPosition().x - letter_sizes[2], cursor.getPosition().y);
    else {
        SizeFiller(font, entryString[cursorPos]);
        letter_sizes = std::move(sizes[entryString[cursorPos]]);
        cursor.setPosition(cursor.getPosition().x - letter_sizes[2], cursor.getPosition().y);
    }
    cursorPos--;

    if (cursorPos + 1 < int(showPos)) {
        showPos--;
        text.setString(wstring(entryString.c_str() + showPos));
        cursor.setPosition(cursor.getPosition().x + letter_sizes[2], cursor.getPosition().y);
    }
}

void Entry::MoveRight() {
    if (cursorPos == entryString.size() - 1)
        return;
    cursorPos++;
    vector<float> letter_sizes = std::move(sizes[entryString[cursorPos]]);
    if (letter_sizes.size())
        cursor.setPosition(cursor.getPosition().x - letter_sizes[2], cursor.getPosition().y);
    else {
        SizeFiller(font, entryString[cursorPos]);
        letter_sizes = std::move(sizes[entryString[cursorPos]]);
        cursor.setPosition(cursor.getPosition().x + letter_sizes[2], cursor.getPosition().y);
    }

    while (cursor.getPosition().x >= text.getGlobalBounds().left + size.x * 0.96f) {
        showPos++;
        text.setString(wstring(entryString.c_str() + showPos));
        letter_sizes = std::move(sizes[entryString[showPos - 1]]);
        if (letter_sizes.size())
            cursor.setPosition(cursor.getPosition().x - letter_sizes[2], cursor.getPosition().y);
        else {
            SizeFiller(font, entryString[showPos - 1]);
            letter_sizes = std::move(sizes[entryString[showPos - 1]]);
            cursor.setPosition(cursor.getPosition().x - letter_sizes[2], cursor.getPosition().y);
        }
    }
}

void Entry::HandleEvent(sf::Event event) {
    if (event.type == sf::Event::KeyPressed) {
        if (event.key.code == sf::Keyboard::BackSpace)
            DeleteSymbol();
        if (event.key.code == sf::Keyboard::Left)
            MoveLeft();
        if (event.key.code == sf::Keyboard::Right)
            MoveRight();
    }
    if (event.type == sf::Event::TextEntered) {
        wchar_t c = wchar_t(event.text.unicode);
        if (c != '\r' && c != '\t' && c != '\b')
            SetSymbol(c);
    }
}

void Entry::SetPosition(const sf::Vector2f pos) {
    //text.setPosition(pos);
    Widget::SetPosition(pos);

    cursor.setPosition(text.getGlobalBounds().width + pos.x, pos.y + 2);
}
void Entry::SetPosition(const float x, const float y) {
    //text.setPosition(x, y);
    Widget::SetPosition(x, y);

    cursor.setPosition(text.getGlobalBounds().width + x, y + 2);
}