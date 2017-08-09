#include "Entry.hpp"

#include <iostream>

using std::vector;
using std::wstring;

Entry::Entry(sf::Vector2f &size) {
    SetSize(size);

    active = false;

    showPos = 0;
    cursorPos = -1;

    cursorTime = sf::Time::Zero;
}

Entry::Entry(sf::Vector2f &size, const sf::Color &color, const sf::Font &font, bool hidingSymbols, wchar_t hidingSymbol)
    : size(size), color(color), font(font), text("", font, 16), hidingSymbols(hidingSymbols), hidingSymbol(hidingSymbol) {
	SetSize(size);

    active = false;

    showPos = 0;
    cursorPos = -1;

    characterSize = text.getCharacterSize();

    cursorTime = sf::Time::Zero;
    cursor.setSize(sf::Vector2f(characterSize / 8.0f, float(characterSize)));
    cursor.setFillColor(color);

    text.setFillColor(color);
    text.setOutlineColor(color);

    cursor.setPosition(text.getGlobalBounds().width, 2);
}

void Entry::AddOnEnterFunc(std::function<void()> func) {
    onEnterFunc = func;
}

vector<float> Entry::getLetterSizes(wchar_t c) {
    //vector<float> letter_sizes = std::move(CC::Get()->GetWindow()->GetUI()->LettersSizes[c]);
    //if (letter_sizes.size())
        //return std::move(letter_sizes);
    //std::cout << "Loading" << std::endl;
    //sizeFiller(font, c);
    //letter_sizes = std::move(CC::Get()->GetWindow()->GetUI()->LettersSizes[c]);
    vector<float> letter_sizes;
    letter_sizes.push_back(font.getGlyph(c, characterSize, false).advance);
    letter_sizes.push_back(font.getGlyph(c, characterSize, true).advance);
    return std::move(letter_sizes);
}

void Entry::moveCursorLeft(wchar_t c) {
    cursor.setPosition(cursor.getPosition().x - font.getGlyph(c, characterSize, false).advance, cursor.getPosition().y);
}

void Entry::moveCursorRight(wchar_t c) {
    //vector<float> letter_sizes = std::move(CC::Get()->GetWindow()->GetUI()->LettersSizes[c]);
    //if (letter_sizes.size())
        //cursor.setPosition(cursor.getPosition().x + letter_sizes[0], cursor.getPosition().y);
    //else {
        //std::cout << "Loading" << std::endl;
        //sizeFiller(font, c);
        //letter_sizes = std::move(CC::Get()->GetWindow()->GetUI()->LettersSizes[c]);
    cursor.setPosition(cursor.getPosition().x + font.getGlyph(c, characterSize, false).advance, cursor.getPosition().y);
    //}
}

void Entry::GrabFocus() { active = true; }
void Entry::LoseFocus() { active = false; }
bool Entry::HasFocus() { return active; }

void Entry::draw() const {
    sf::RenderTexture &buffer = const_cast<sf::RenderTexture &>(this->buffer);
    //buffer.clear(sf::Color(31, 31, 31));
    buffer.clear(sf::Color(60, 60, 60));
    //buffer.clear(sf::Color::White);
    buffer.draw(text);

    //std::cout << color.toInteger() << ' ' << cursor.getPosition().x << ' ' << cursor.getPosition().y << ' ' << GetAbsPosition().x << ' ' << GetAbsPosition().y << std::endl;
   
   if (active && cursorTime >= sf::seconds(0.6f) && cursorTime <= sf::seconds(0.8f))
        buffer.draw(cursor);

    buffer.display();
}

void Entry::Update(sf::Time timeElapsed) {
    if (active) {
        cursorTime += timeElapsed;

        if (cursorTime > sf::seconds(0.8f))
            cursorTime = sf::Time::Zero;
    }
}

void Entry::setSymbol(wchar_t c) {
    if (hidingSymbols) {
        hidingString += c;
        c = hidingSymbol;
    }
    if (cursorPos == entryString.size() - 1)
        entryString += c;
    else
        entryString.insert(entryString.begin() + 1 + cursorPos, c);

    cursorPos += 1;
    moveCursorRight(c);

    while (cursor.getPosition().x >= text.getGlobalBounds().left + size.x * 0.96f) {
        showPos++;
        text.setString(wstring(entryString.c_str() + showPos));
        moveCursorLeft(entryString[showPos - 1]);
    }

    text.setString(wstring(entryString.c_str() + showPos));
}

void Entry::deleteSymbol() {
    if (cursorPos < 0)
        return;
    moveCursorLeft(entryString[cursorPos]);
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
        for (unsigned i = showPos; i < showPos + shift_size; i++)
            moveCursorRight(entryString[i]);
    }

    text.setString(wstring(entryString.c_str() + showPos));
}

void Entry::moveLeft() {
    if (cursorPos < 0)
        return;
    moveCursorLeft(entryString[cursorPos]);
    vector<float> letter_sizes = getLetterSizes(entryString[cursorPos]);
    cursorPos--;

    if (cursorPos + 1 < int(showPos)) {
        showPos--;
        text.setString(wstring(entryString.c_str() + showPos));
        cursor.setPosition(cursor.getPosition().x + letter_sizes[0], cursor.getPosition().y);
    }
}

void Entry::moveRight() {
    if (cursorPos == entryString.size() - 1)
        return;

    cursorPos++;
    moveCursorRight(entryString[cursorPos]);

    while (cursor.getPosition().x >= text.getGlobalBounds().left + size.x * 0.96f) {
        showPos++;
        text.setString(wstring(entryString.c_str() + showPos));
        moveCursorLeft(entryString[showPos - 1]);
    }
}

void Entry::Clear() {
    text.setString("");
    entryString.erase(entryString.begin(), entryString.end());
    hidingString.erase(hidingString.begin(), hidingString.end());
    showPos = 0;
    cursorPos = -1;

    cursor.setPosition(text.getGlobalBounds().width, cursor.getPosition().y);
}

std::string Entry::GetText() {
    std::string returnedString(entryString.size() * 4, 0);
    std::string::iterator end = sf::Utf<8>::fromWide(entryString.begin(), entryString.end(), returnedString.begin());
    if (hidingSymbols)
        end = sf::Utf<8>::fromWide(hidingString.begin(), hidingString.end(), returnedString.begin());
    returnedString.erase(end, returnedString.end());

    text.setString("");
    entryString.erase(entryString.begin(), entryString.end());
    hidingString.erase(hidingString.begin(), hidingString.end());
    showPos = 0;
    cursorPos = -1;

    cursor.setPosition(text.getGlobalBounds().width, cursor.getPosition().y);

    //std::cout << returnedString << std::endl;

    return returnedString;
}

void Entry::HideSymbols(wchar_t hider) {
    hidingSymbol = hider;
    hidingSymbols = true;
    hidingString = entryString;
    entryString = std::wstring(entryString.size(), hidingSymbol);
}

void Entry::ShowSymbols() {
    hidingSymbols = false;
    std::swap(hidingString, entryString);
}

bool Entry::HandleEvent(sf::Event event) {
    int x = event.mouseButton.x,
        y = event.mouseButton.y;

    if (event.type == sf::Event::MouseButtonPressed && x >= GetAbsPosition().x && x <= GetAbsPosition().x + GetSize().x &&
        y >= GetAbsPosition().y && y <= GetAbsPosition().y + GetSize().y) {
        GrabFocus();
        return true;
    }

    if (active) {
        if (event.type == sf::Event::KeyPressed) {
            if (event.key.code == sf::Keyboard::BackSpace)
                deleteSymbol();
            if (event.key.code == sf::Keyboard::Left)
                moveLeft();
            if (event.key.code == sf::Keyboard::Right)
                moveRight();
            if (event.key.code == sf::Keyboard::Return)
                onEnterFunc();
        }
        if (event.type == sf::Event::TextEntered) {
            wchar_t c = wchar_t(event.text.unicode);
            if (c != '\r' && c != '\t' && c != '\b')
                setSymbol(c);
        }
        return true;
    }

    return false;
}

//void Entry::SetPosition(const sf::Vector2f pos) {
//    //text.setPosition(pos);
//    Widget::SetPosition(pos);
//
//    cursor.setPosition(text.getGlobalBounds().width, 2);
//}
//void Entry::SetPosition(const float x, const float y) {
//    //text.setPosition(x, y);
//    Widget::SetPosition(x, y);
//
//    cursor.setPosition(text.getGlobalBounds().width, 2);
//}

//void Entry::sizeFiller(const sf::Font &font, sf::Uint32 c) {
//    //sizes[c].push_back(font.getGlyph(c, characterSize, false).bounds.width);
//    //sizes[c].push_back(font.getGlyph(c, characterSize, true).bounds.width);
//    font.getGlyph(c, characterSize, false).advance;
//    font.getGlyph(c, characterSize, true).advance;
//}