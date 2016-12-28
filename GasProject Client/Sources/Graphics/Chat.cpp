#include "Graphics/Chat.hpp"
#include "Graphics/Window.hpp"
#include "Network.hpp"

Chat::Chat(const sf::Font &font) {
    characterSize = 18;

    cout << font.getLineSpacing(characterSize) << endl;
    for (sf::Uint32 c = L'a'; c <= L'z'; c++) {
        //sizes[c].first = font.getGlyph(c, characterSize, false).bounds.width;
        //sizes[c].second = font.getGlyph(c, characterSize, true).bounds.width;
        sizes[c].first = float(font.getGlyph(c, characterSize, false).textureRect.width);
        sizes[c].second = float(font.getGlyph(c, characterSize, true).textureRect.width);
        //cout << c << ' ' << sizes[c].first << ' ' << sizes[c].second << endl;
    }

    entry.setFillColor(sf::Color(31, 31, 31));
    box.setFillColor(sf::Color(60, 60, 60));

    sf::Color textColor = sf::Color(193, 205, 205);

    entryText.setFont(font);
    entryText.setCharacterSize(characterSize);
    entryText.setFillColor(textColor);
    entryText.setString("");

    showPos = 0;
    resized = false;

    active = false;
    cursorTime = sf::Time::Zero;
    cursor.setFillColor(textColor);
    cursor.setSize(sf::Vector2f(characterSize / 6.0f, characterSize + 4.0f));

    //wstring
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
            size += boxText[i].text[j][0].getGlobalBounds().height + characterSize - 5;
            if (size > box.getSize().y)
                break;
            float boxTextXShift = box.getSize().x * 0.01f;
            for (size_t k = 0; k < boxText[i].text[j].size(); k++) {
                boxText[i].text[j][k].setPosition(chatXPos + boxTextXShift, chatYPos + box.getSize().y - size);
                renderWindow->draw(boxText[i].text[j][k]);
                boxTextXShift += boxText[i].text[j][k].getGlobalBounds().width;
            }
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

    cursor.setPosition(chatXPos + entryTextXShift, chatYPos + box.getSize().y + entryTextYShift - 2);
}

void Chat::AddIncomingMessage(const std::string &message) {
    wstring result(message.size(), 0);
    wstring::iterator end = sf::Utf<8>::toWide(message.begin(), message.end(), result.begin());
    result.erase(end, result.end());

    mtx.lock();
    boxText.push_back(Message(result));
    mtx.unlock();
}

void Chat::Update() {
    sf::Text tempText;
    tempText.setFont(*entryText.getFont());
    tempText.setCharacterSize(entryText.getCharacterSize());
    tempText.setFillColor(entryText.getFillColor());
    tempText.setScale(entryText.getScale());

    int iter = int(boxText.size());
    if (!iter)
        return;

    mtx.lock();
    while (boxText[--iter].text.empty()) {
        //tempText.setString('>' + boxText[iter].playerName + ':');
        //boxText[iter].text.push_back(tempText);
        Parse(boxText[iter], tempText);
        if (!iter)
            break;
    }
    mtx.unlock();

    while (boxText.size() > 100)
        boxText.erase(boxText.begin());

    if (resized) {
        for (auto &message : boxText) {
            //if (message.text.size() > 2) {
            if (message.text.size() > 1) {
                //message.text.erase(++message.text.begin(), message.text.end());
                message.text.erase(message.text.begin(), message.text.end());
                Parse(message, tempText);
            }
            //else if (message.text[1].getGlobalBounds().width >= entry.getSize().x) {
            else {
                float size = 0;
                for (auto &i : message.text[0])
                    size += i.getGlobalBounds().width;
                if (size >= entry.getSize().x) {
                    //message.text.erase(++message.text.begin());
                    message.text.erase(message.text.begin());
                    Parse(message, tempText);
                }
            }
        }
        resized = false;
    }
}

void Chat::Resize(int width, int height) {
    float cursYPos = cursor.getPosition().y - chatYPos - box.getSize().y - entryTextYShift + 2;

    TileGrid *tileGrid = CC::Get()->GetWindow()->GetTileGrid();

    chatXPos = tileGrid->GetTileSize() * float(Global::FOV);

    float chatWidth = width - chatXPos;
    float entryHeight = height * 0.05f;
    float chatShift = entryHeight * 0.1f;

    //sf::Vector2f scale = sf::Vector2f(float(chatWidth) / entry.getSize().x, float(chatWidth) / entry.getSize().x);
    //for (auto &message : boxText)
        //for (auto &text : message.text)
            //text.setScale(text.getScale().x * scale);
    if (chatWidth != entry.getSize().x)
        resized = true;

    entry.setSize(sf::Vector2f(chatWidth, entryHeight));
    box.setSize(sf::Vector2f(chatWidth, height * 0.5f - entryHeight));

    chatYPos = height - entry.getSize().y - box.getSize().y - chatShift;
    entry.setPosition(chatXPos, chatYPos + box.getSize().y);
    box.setPosition(chatXPos, chatYPos);

    entryTextXShift = entry.getSize().x * 0.01f, entryTextYShift = entry.getSize().y * 0.1f;
    entryText.setPosition(chatXPos + entryTextXShift, chatYPos + box.getSize().y + entryTextYShift);
    //static bool resize = false;
    //if (resize) {
        //entryText.setScale(entryText.getScale().x * scale);
        //cursor.setScale(cursor.getScale().x * scale);
    //}
    //else
        //resize = true;

    cursor.setPosition(entryText.getGlobalBounds().width + chatXPos + entryTextXShift, cursYPos + chatYPos + box.getSize().y + entryTextYShift - 2);
}

void Chat::Parse(Message &message, sf::Text &tempText) {
    unsigned startPos = 0, shiftPos = 0;

    wstring playerName;
    float nameWidth = 0;
    if (message.stringText[0] == '<') {
        int counter = 0;
        while (message.stringText[counter] != '>')
            counter++;
        playerName = message.stringText.substr(1, counter - 1);
        playerName += L": ";
        message.stringText.erase(message.stringText.begin(), message.stringText.begin() + counter + 1);
    }
    if (!playerName.empty()) {
        tempText.setStyle(sf::Text::Bold);
        tempText.setString(playerName);
        message.text.push_back(vector<sf::Text>(1, tempText));
        nameWidth += tempText.getGlobalBounds().width;
    }

    tempText.setStyle(sf::Text::Regular);
    size_t iter = 0;
    while (startPos < message.stringText.size()) {
        //float size = 0;
        //while (size < entry.getSize().x * 0.96f && startPos + shiftPos < unsigned(message.stringText.size())) {
        do {
            shiftPos++;
            //size += sizes[message.stringText[startPos + shiftPos]].first;
            //cout << int(message.stringText[startPos + shiftPos]) << ' ' << sizes[message.stringText[startPos + shiftPos]].first << endl;
            tempText.setString(message.stringText.substr(startPos, shiftPos));
        } while (tempText.getGlobalBounds().width + nameWidth < entry.getSize().x * 0.96f && startPos + shiftPos < unsigned(message.stringText.size()));

        nameWidth = 0;

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
            }
        
        tempText.setString(message.stringText.substr(startPos, shiftPos));

        if (message.text.size() < ++iter)
            message.text.push_back(vector<sf::Text>(1, tempText));
        else
            message.text[iter - 1].push_back(tempText);

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
            size += boxText[i].text[j][0].getGlobalBounds().height + characterSize - 5;
            if (size > box.getSize().y)
                break;
            float boxTextXShift = box.getSize().x * 0.01f;
            //boxText[i].text[j].setPosition(chatXPos + boxTextXShift, chatYPos + box.getSize().y - size);
            //renderWindow->draw(boxText[i].text[j]);
            
            //target.draw(boxText[i].text[j]);
        }
        if (size > box.getSize().y)
            break;
    }
    //mtx.unlock();
}