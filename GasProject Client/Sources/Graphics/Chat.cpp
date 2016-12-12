#include "Graphics/Chat.hpp"
#include "Graphics/Window.hpp"
#include "Network.hpp"

Chat::Chat(const sf::Font &font) {
    entry.setFillColor(sf::Color(31, 31, 31));
    box.setFillColor(sf::Color(60, 60, 60));

    sf::Color textColor = sf::Color(193, 205, 205);
    characterSize = 18;

    entryText.setFont(font);
    entryText.setCharacterSize(characterSize);
    entryText.setFillColor(textColor);
    entryText.setString("");

    showPos = 0;

    active = false;
    cursorTime = sf::Time::Zero;
    cursor.setFillColor(textColor);
    cursor.setSize(sf::Vector2f(characterSize / 6.0f, characterSize + 4.0f));
}

void Chat::Draw(sf::RenderWindow *renderWindow, sf::Time timeElapsed) {
    renderWindow->draw(entry);
    renderWindow->draw(box);

    renderWindow->draw(entryText);

    cursorTime += timeElapsed;
    if (active && cursorTime >= sf::seconds(0.8f) && cursorTime <= sf::seconds(1))
        renderWindow->draw(cursor);
    if (cursorTime > sf::seconds(1))
        cursorTime = sf::Time::Zero;

    float size = 0;
    mtx.lock();
    for (int i = int(boxText.size() - 1); i >= 0; i--) {
        for (int j = int(boxText[i].text.size() - 1); j >= 0; j--) {
            size += boxText[i].text[j].getGlobalBounds().height + characterSize - 5;
            if (size > box.getSize().y)
                break;
            float boxTextXShift = box.getSize().x * 0.01f;
            boxText[i].text[j].setPosition(chatXPos + boxTextXShift, chatYPos + box.getSize().y - size);
            renderWindow->draw(boxText[i].text[j]);
        }
        if (size > box.getSize().y)
            break;
    }
    mtx.unlock();
}

void Chat::SetSymbol(const wchar_t c) {
    entryString += c;

    sf::Text tempText;
    tempText.setFont(*entryText.getFont());
    tempText.setCharacterSize(entryText.getCharacterSize());
    tempText.setString(wstring(entryString.c_str() + showPos));
    tempText.setScale(entryText.getScale());

    while (tempText.getGlobalBounds().width >= entry.getSize().x * 0.98)
        showPos++, tempText.setString(wstring(entryString.c_str() + showPos));

    cursor.setPosition(cursor.getPosition().x + tempText.getGlobalBounds().width - entryText.getGlobalBounds().width, cursor.getPosition().y);
    entryText.setString(wstring(entryString.c_str() + showPos));
}

void Chat::DeleteSymbol() {
    if (entryString.size())
        entryString.resize(entryString.size() - 1);

    sf::Text tempText;
    tempText.setFont(*entryText.getFont());
    tempText.setCharacterSize(entryText.getCharacterSize());
    tempText.setScale(entryText.getScale());
    tempText.setString(wstring(entryString.c_str() + showPos));

    if (showPos) {
        showPos--;
        tempText.setString(wstring(entryString.c_str() + showPos));
        if (tempText.getGlobalBounds().width >= entry.getSize().x * 0.98)
            showPos++;
        tempText.setString(wstring(entryString.c_str() + showPos));
    }

    cursor.setPosition(cursor.getPosition().x + tempText.getGlobalBounds().width - entryText.getGlobalBounds().width, cursor.getPosition().y);
    
    entryText.setString(wstring(entryString.c_str() + showPos));
}

void Chat::Send() {
    if (entryString.size()) {
        std::string message(entryString.size() * 4, 0);
        std::string::iterator end = sf::Utf<8>::fromWide(entryString.begin(), entryString.end(), message.begin());
        message.erase(end, message.end());
        Connection::commandQueue.Push(new SendChatMessageClientCommand(message));
    }

    entryText.setString("");
    entryString.erase(entryString.begin(), entryString.end());
    showPos = 0;

    float entryTextXShift = entry.getSize().x * 0.01f, entryTextYShift = entry.getSize().y * 0.1f;
    cursor.setPosition(chatXPos + entryTextXShift, chatYPos + box.getSize().y + entryTextYShift - 2);
}

void Chat::AddIncomingMessage(const std::string &message, const std::string &playerName) {
    wstring result(message.size(), 0);
    wstring::iterator end = sf::Utf<8>::toWide(message.begin(), message.end(), result.begin());
    result.erase(end, result.end());

    mtx.lock();
    boxText.push_back(Message(playerName, result));
    mtx.unlock();
}

void Chat::Update() {
    sf::Text tempText;
    tempText.setFont(*entryText.getFont());
    tempText.setCharacterSize(entryText.getCharacterSize());
    tempText.setFillColor(entryText.getFillColor());
    tempText.setScale(entryText.getScale());

    int iter = int(boxText.size() - 1); 
    if (iter < 0)
        return;

    while (boxText[iter].text.empty()) {
        tempText.setString('>' + boxText[iter].playerName + ':');
        boxText[iter].text.push_back(tempText);
        Parse(boxText[iter], tempText);
    }

    while (boxText.size() > 100)
        boxText.erase(boxText.begin());
}

void Chat::Resize(int width, int height) {
    TileGrid *tileGrid = CC::Get()->GetWindow()->GetTileGrid();

    chatXPos = tileGrid->GetTileSize() * float(Global::FOV);

    float chatWidth = width - chatXPos;
    float entryHeight = height * 0.05f;
    float chatShift = entryHeight * 0.1f;

    sf::Vector2f scale = sf::Vector2f(float(chatWidth) / entry.getSize().x, float(chatWidth) / entry.getSize().x);
    for (auto &message : boxText)
        for (auto &text : message.text)
            text.setScale(text.getScale().x * scale);

    entry.setSize(sf::Vector2f(chatWidth, entryHeight));
    box.setSize(sf::Vector2f(chatWidth, height * 0.5f - entryHeight));

    chatYPos = height - entry.getSize().y - box.getSize().y - chatShift;
    entry.setPosition(chatXPos, chatYPos + box.getSize().y);
    box.setPosition(chatXPos, chatYPos);

    float entryTextXShift = entry.getSize().x * 0.01f, entryTextYShift = entry.getSize().y * 0.1f;
    entryText.setPosition(chatXPos + entryTextXShift, chatYPos + box.getSize().y + entryTextYShift);
    static bool resize = false;
    if (resize)
        entryText.setScale(entryText.getScale().x * scale);
    else
        resize = true;

    cursor.setPosition(chatXPos + entryTextXShift, chatYPos + box.getSize().y + entryTextYShift - 2);
}

void Chat::Parse(Message &message, sf::Text &tempText) {
    unsigned startPos = 0, shiftPos = 0;
    while (startPos < message.stringText.size()) {
        do {
            shiftPos++;
            tempText.setString(message.stringText.substr(startPos, shiftPos));
        } while (tempText.getGlobalBounds().width < entry.getSize().x * 0.96f && startPos + shiftPos <= unsigned(message.stringText.size()));

        bool spaces = true;
        if (startPos + shiftPos < message.stringText.size())
            if (message.stringText[startPos + shiftPos] != ' ') {
                spaces = false;
                for (unsigned i = startPos + shiftPos; i > startPos; i--) {
                    if (message.stringText[i] == ' ') {
                        spaces = true;
                        shiftPos = i - startPos;
                        break;
                    }
                }
                tempText.setString(message.stringText.substr(startPos, shiftPos));
            }

        message.text.push_back(tempText);
        startPos += shiftPos;
        if (spaces)
            startPos++;
        shiftPos = 1;
    }
}

void Chat::draw(sf::RenderTarget &target, sf::RenderStates states) const {
    target.draw(entry);
    target.draw(box);
    //renderWindow->draw(entry);
    //renderWindow->draw(box);

    //renderWindow->draw(entryText);
    target.draw(entryText);

    float size = 0;
    //mtx.lock();
    for (int i = int(boxText.size() - 1); i >= 0; i--) {
        for (int j = int(boxText[i].text.size() - 1); j >= 0; j--) {
            size += boxText[i].text[j].getGlobalBounds().height + characterSize - 5;
            if (size > box.getSize().y)
                break;
            float boxTextXShift = box.getSize().x * 0.01f;
            //boxText[i].text[j].setPosition(chatXPos + boxTextXShift, chatYPos + box.getSize().y - size);
            //renderWindow->draw(boxText[i].text[j]);
            target.draw(boxText[i].text[j]);
        }
        if (size > box.getSize().y)
            break;
    }
    //mtx.unlock();
}