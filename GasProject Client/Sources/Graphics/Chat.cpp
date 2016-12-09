#include "Graphics/Chat.hpp"
#include "Graphics/Window.hpp"
#include "Network.hpp"

Chat::Chat(const sf::Font &font) {
    entry.setFillColor(sf::Color(31, 31, 31));
    box.setFillColor(sf::Color(60, 60, 60));

    entryText.setFont(font);
    entryText.setCharacterSize(20);
    entryText.setFillColor(sf::Color::Magenta);
    entryText.setString("");
    showPos = 0;
}

void Chat::Draw(sf::RenderWindow *renderWindow) {
    renderWindow->draw(entry);
    renderWindow->draw(box);

    renderWindow->draw(entryText);

    float size = 0;
    for (int i = int(boxText.size() - 1); i >= 0; i--) {
        for (int j = int(boxText[i].text.size() - 1); j >= 0; j--) {
            size += boxText[i].text[j].getGlobalBounds().height + 15;
            if (size > box.getSize().y)
                break;
            float boxTextXShift = box.getSize().x * 0.01f;
            boxText[i].text[j].setPosition(chatXPos + boxTextXShift, chatYPos - size);
            renderWindow->draw(boxText[i].text[j]);
        }
        if (size > box.getSize().y)
            break;
    }
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

    entryText.setString(wstring(entryString.c_str() + showPos));
}

void Chat::Send() {
    Connection::commandQueue.Push(new SendChatMessageClientCommand(entryString));

    entryText.setString("");
    entryString.erase(entryString.begin(), entryString.end());
    showPos = 0;
}

void Chat::DeleteSymbol() {
    if (entryString.size())
        entryString.resize(entryString.size() - 1);

    if (showPos) {
        showPos--;
        sf::Text tempText;
        tempText.setFont(*entryText.getFont());
        tempText.setCharacterSize(entryText.getCharacterSize());
        tempText.setScale(entryText.getScale());
        tempText.setString(wstring(entryString.c_str() + showPos));
        if (tempText.getGlobalBounds().width >= entry.getSize().x * 0.98)
            showPos++;
    }
    
    entryText.setString(wstring(entryString.c_str() + showPos));
}

void Chat::AddIncomingMessage(const wstring &message, const std::string &playerName) {
    boxText.push_back(Message(playerName, message));
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

    chatYPos = height - entry.getSize().y - chatShift;
    entry.setPosition(chatXPos, chatYPos);
    box.setPosition(chatXPos, chatYPos - box.getSize().y);

    float entryTextXShift = entry.getSize().x * 0.01f, entryTextYShift = entry.getSize().y * 0.1f;
    entryText.setPosition(chatXPos + entryTextXShift, chatYPos + entryTextYShift);
    static bool resize = false;
    if (resize)
        entryText.setScale(entryText.getScale().x * scale);
    else
        resize = true;
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