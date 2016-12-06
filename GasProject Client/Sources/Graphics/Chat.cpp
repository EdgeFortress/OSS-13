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
    if (boxText.size())
        for (int i = int(boxText.size() - 1); i >= 0; i--) {
            size += boxText[i].getLocalBounds().height + 15;
            if (size > box.getSize().y)
                break;
            float boxTextXShift = box.getSize().x * 0.01f;
            boxText[i].setPosition(chatXPos + boxTextXShift, chatYPos - size);
            renderWindow->draw(boxText[i]);
        }
}

void Chat::SetSymbol(const wchar_t c) {
    entryString += c;

    sf::Text tempText;
    tempText.setFont(*entryText.getFont());
    tempText.setCharacterSize(20);
    tempText.setString(wstring(entryString.c_str() + showPos));

    while (tempText.getLocalBounds().width >= entry.getSize().x * 0.98)
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
        tempText.setCharacterSize(20);
        tempText.setString(wstring(entryString.c_str() + showPos));
        if (tempText.getLocalBounds().width >= entry.getSize().x * 0.98)
            showPos++;
    }
    
    entryText.setString(wstring(entryString.c_str() + showPos));
}

void Chat::AddIncomingMessage(wstring &message, const std::string &playerName) {
    sf::Text tempText;
    tempText.setFont(*entryText.getFont());
    tempText.setCharacterSize(20);
    tempText.setFillColor(sf::Color::Magenta);
    tempText.setString('>' + playerName + ':');
    boxText.push_back(tempText);

    static int counter = 0;
    //CC::log << "GOT MESSAGE" << counter << endl;

    unsigned startPos = 0, shiftPos = 0;
    while (startPos < message.size()) {
        do {
            shiftPos++;
            tempText.setString(message.substr(startPos, shiftPos));
            float fieldSize = entry.getSize().x * 0.96f;
            //wcout << message << endl;
            unsigned messageSize = unsigned(message.size());
            //wcout << message.substr(startPos, shiftPos) << endl;
            //sf::FloatRect textRect = tempText.getGlobalBounds();
            //CC::log << shiftPos << endl;
            float textSize = shiftPos * 15.0f;//textRect.width;
            //CC::log << "CYCLE" << counter << endl;
            if (textSize >= fieldSize || startPos + shiftPos > messageSize)
                break;
        } while (true);

        //CC::log << "PARSING MESSAGE" << counter << endl;

        bool spaces = true;
        if (startPos + shiftPos < message.size())
            if (message[startPos + shiftPos] != ' ') {
                spaces = false;
                for (unsigned i = startPos + shiftPos; i > startPos; i--) {
                    if (message[i] == ' ') {
                        spaces = true;
                        shiftPos = i - startPos;
                        break;
                    }
                }
                tempText.setString(message.substr(startPos, shiftPos));
            }

        boxText.push_back(tempText);
        startPos += shiftPos;
        if (spaces)
            startPos++;
        shiftPos = 1;
    }

    //CC::log << "FINISHED PARSING" << counter << endl;

    counter++;
}

void Chat::Resize(int width, int height) {
    TileGrid *tileGrid = CC::Get()->GetWindow()->GetTileGrid();

    chatXPos = tileGrid->GetTileSize() * float(Global::FOV);

    float chatWidth = width - chatXPos;
    float entryHeight = height * 0.05f;
    float chatShift = entryHeight * 0.1f;

    entry.setSize(sf::Vector2f(chatWidth, entryHeight));
    box.setSize(sf::Vector2f(chatWidth, height * 0.5f - entryHeight));

    chatYPos = height - entry.getSize().y - chatShift;
    entry.setPosition(chatXPos, chatYPos);
    box.setPosition(chatXPos, chatYPos - box.getSize().y);

    float entryTextXShift = entry.getSize().x * 0.01f, entryTextYShift = entry.getSize().y * 0.1f;
    entryText.setPosition(chatXPos + entryTextXShift, chatYPos + entryTextYShift);
}