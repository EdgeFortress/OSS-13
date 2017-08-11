#include "Entry.hpp"

#include <iostream>

#include "Shared/Geometry/Vec2.hpp"

#include "Client.hpp"

Entry::Entry(sf::Vector2f &size) {
    SetSize(size);

    active = false;

    showPos = 0;
    cursorPos = -1;

    cursorTime = sf::Time::Zero;
}

Entry::Entry(sf::Vector2f &size, const sf::Color &color, const sf::Font &font, bool hidingSymbols, wchar_t hidingSymbol)
    : color(color), font(font), text("", font, 16), hidingSymbols(hidingSymbols), hidingSymbol(hidingSymbol) {
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

void Entry::SetOnEnterFunc(std::function<void()> func) {
    onEnterFunc = func;
}

std::vector<float> Entry::getLetterSizes(wchar_t c) {
    //vector<float> letter_sizes = std::move(CC::Get()->GetWindow()->GetUI()->LettersSizes[c]);
    //if (letter_sizes.size())
        //return std::move(letter_sizes);
    //std::cout << "Loading" << std::endl;
    //sizeFiller(font, c);
    //letter_sizes = std::move(CC::Get()->GetWindow()->GetUI()->LettersSizes[c]);
    std::vector<float> letter_sizes;
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

void Entry::SetActive(bool active) { this->active = active; }
bool Entry::IsActive() { return active; }

void Entry::draw() const {
    buffer.clear(sf::Color(60, 60, 60));
    buffer.draw(text);
   
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

    while (cursor.getPosition().x >= text.getGlobalBounds().left + GetSize().x * 0.96f) {
        showPos++;
        text.setString(std::wstring(entryString.c_str() + showPos));
        moveCursorLeft(entryString[showPos - 1]);
    }

    text.setString(std::wstring(entryString.c_str() + showPos));
}

void Entry::deleteSymbol() {
    if (cursorPos < 0)
        return;
    moveCursorLeft(entryString[cursorPos]);
    entryString.erase(entryString.begin() + cursorPos);
    text.setString(std::wstring(entryString.c_str() + showPos));

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

    text.setString(std::wstring(entryString.c_str() + showPos));
}

void Entry::moveLeft() {
    if (cursorPos < 0)
        return;
    moveCursorLeft(entryString[cursorPos]);
    std::vector<float> letter_sizes = getLetterSizes(entryString[cursorPos]);
    cursorPos--;

    if (cursorPos + 1 < int(showPos)) {
        showPos--;
        text.setString(std::wstring(entryString.c_str() + showPos));
        cursor.setPosition(cursor.getPosition().x + letter_sizes[0], cursor.getPosition().y);
    }
}

void Entry::moveRight() {
    if (cursorPos == entryString.size() - 1)
        return;

    cursorPos++;
    moveCursorRight(entryString[cursorPos]);

    while (cursor.getPosition().x >= text.getGlobalBounds().left + GetSize().x * 0.96f) {
        showPos++;
        text.setString(std::wstring(entryString.c_str() + showPos));
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
	switch (event.type) {
	case sf::Event::MouseButtonPressed: {
		uf::vec2i mousePosition = uf::vec2i(event.mouseButton.x, event.mouseButton.y);
		if (mousePosition >= GetAbsPosition() && mousePosition < GetAbsPosition() + GetSize()) {
			SetActive(true);
			return true;
		}
		return false;
	}
	case sf::Event::KeyPressed: {
		if (active) {
			switch (event.key.code) {
			case sf::Keyboard::BackSpace:
				deleteSymbol();
				break;
			case sf::Keyboard::Left:
				moveLeft();
				break;
			case sf::Keyboard::Right:
				moveRight();
				break;
			case sf::Keyboard::Return:
				onEnterFunc();
				break;
			default:
				return false;
			}
			return true;
		}
	}
	case sf::Event::TextEntered: {
		if (active) {
			wchar_t c = wchar_t(event.text.unicode);
			if (c != '\r' && c != '\t' && c != '\b')
				setSymbol(c);
			return true;
		}
	}
	}

    return false;
}