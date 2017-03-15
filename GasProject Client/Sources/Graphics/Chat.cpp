#include "Graphics/Chat.hpp"
#include "Graphics/Window.hpp"

#include "Network.hpp"

Chat::Chat(const sf::Font &font) : font(font) {
    characterSize = 18;

    for (sf::Uint32 c = L'a'; c <= L'z'; c++)
        SizeFiller(font, c);
    for (sf::Uint32 c = L'A'; c <= L'Z'; c++)
        SizeFiller(font, c);

    entry.setFillColor(sf::Color(31, 31, 31));
    box.setFillColor(sf::Color(60, 60, 60));

    sf::Color textColor = sf::Color(193, 205, 205);

    entryText.setFont(font);
    entryText.setCharacterSize(characterSize);
    entryText.setFillColor(textColor);
    entryText.setString("");

    showPos = 0;
    cursorPos = -1;
    resized = false;
    scrolled = 0;

    active = false;
    cursorTime = sf::Time::Zero;
    cursor.setFillColor(textColor);
    cursor.setSize(sf::Vector2f(characterSize / 6.0f, float(characterSize)));
}

void Chat::SizeFiller(const sf::Font &font, sf::Uint32 c) {
    sizes[c].push_back(font.getGlyph(c, characterSize, false).bounds.width);
    sizes[c].push_back(font.getGlyph(c, characterSize, true).bounds.width);
    sizes[c].push_back(font.getGlyph(c, characterSize, false).advance);
    sizes[c].push_back(font.getGlyph(c, characterSize, true).advance);
}

void Chat::Draw(sf::RenderWindow *renderWindow, sf::Time timeElapsed) {
    renderWindow->draw(entry);
    renderWindow->draw(box);

    renderWindow->draw(entryText);

    cursorTime += timeElapsed;
    if (active && cursorTime >= sf::seconds(0.6f) && cursorTime <= sf::seconds(0.8f))
        renderWindow->draw(cursor);
    if (cursorTime > sf::seconds(0.8f))
        cursorTime = sf::Time::Zero;

    int cur_scroll = scrolled;

    float size = 0;
    mtx.lock();
    for (int i = int(boxText.size() - 1); i >= 0; i--) {
        for (int j = int(boxText[i].text.size() - 1); j >= 0; j--) {
            if (!cur_scroll) {
                size += boxText[i].text[j][0].getGlobalBounds().height + characterSize - 5;
                if (size > box.getSize().y)
                    break;
                float boxTextXShift = box.getSize().x * 0.01f;
                for (size_t k = 0; k < boxText[i].text[j].size(); k++) {
                    boxText[i].text[j][k].setPosition(chatXPos + boxTextXShift, chatYPos + box.getSize().y - size);
                    renderWindow->draw(boxText[i].text[j][k]);
                    boxTextXShift += boxText[i].text[j][k].getGlobalBounds().width;
                }
            } else
                cur_scroll--;
        }
        if (size > box.getSize().y)
            break;
    }

    mtx.unlock();
}

void Chat::ScrollUp() {
    int totalSize = 0;
    for (auto &message : boxText)
        totalSize += int(message.text.size());
    if (scrolled < totalSize)
        scrolled++;
}

void Chat::ScrollDown() {
    if (scrolled > 0)
        scrolled--;
}

void Chat::SetSymbol(const wchar_t c) {
    if (cursorPos == entryString.size() - 1)
        entryString += c;
    else
        entryString.insert(entryString.begin() + 1 + cursorPos, c);

    cursorPos += 1;

    vector<float> letter_sizes = std::move(sizes[c]);
    if (letter_sizes.size())
        cursor.setPosition(cursor.getPosition().x + letter_sizes[2], cursor.getPosition().y);
    else {
        SizeFiller(font, c);
        letter_sizes = std::move(sizes[c]);
        cursor.setPosition(cursor.getPosition().x + letter_sizes[2], cursor.getPosition().y);
    }

    while (cursor.getPosition().x >= entryText.getGlobalBounds().left + entry.getGlobalBounds().width * 0.96f) {
        showPos++;
        entryText.setString(wstring(entryString.c_str() + showPos));
        letter_sizes = std::move(sizes[entryString[showPos - 1]]);
        if (letter_sizes.size())
            cursor.setPosition(cursor.getPosition().x - letter_sizes[2], cursor.getPosition().y);
        else {
            SizeFiller(font, entryString[showPos - 1]);
            letter_sizes = std::move(sizes[entryString[showPos - 1]]);
            cursor.setPosition(cursor.getPosition().x - letter_sizes[2], cursor.getPosition().y);
        }
    }

    entryText.setString(wstring(entryString.c_str() + showPos));
}

void Chat::DeleteSymbol() {
    if (cursorPos < 0)
        return;

    vector<float> letter_sizes = std::move(sizes[entryString[cursorPos]]);
    if (letter_sizes.size()) {
        cursor.setPosition(cursor.getPosition().x - letter_sizes[2], cursor.getPosition().y);
    } else {
        SizeFiller(font, entryString[cursorPos]);
        letter_sizes = std::move(sizes[entryString[cursorPos]]);
        cursor.setPosition(cursor.getPosition().x - letter_sizes[2], cursor.getPosition().y);
    }

    entryString.erase(entryString.begin() + cursorPos);
    entryText.setString(wstring(entryString.c_str() + showPos));

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
    
    entryText.setString(wstring(entryString.c_str() + showPos));
}

void Chat::MoveLeft() {
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
        entryText.setString(wstring(entryString.c_str() + showPos));
        cursor.setPosition(cursor.getPosition().x + letter_sizes[2], cursor.getPosition().y);
    }
}

void Chat::MoveRight() {
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

    while (cursor.getPosition().x >= entryText.getGlobalBounds().left + entry.getGlobalBounds().width * 0.96f) {
        showPos++;
        entryText.setString(wstring(entryString.c_str() + showPos));
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
    cursorPos = -1;

    cursor.setPosition(chatXPos + entryTextXShift, cursor.getPosition().y);
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
        Parse(boxText[iter], tempText);
        if (!iter)
            break;
    }
    mtx.unlock();

    while (boxText.size() > 100)
        boxText.erase(boxText.begin());

    if (resized) {
        for (auto &message : boxText) {
            if (message.text.size() > 1) {
                message.text.erase(message.text.begin(), message.text.end());
                Parse(message, tempText);
            }
            else {
                float size = 0;
                for (auto &i : message.text[0])
                    size += i.getGlobalBounds().width;
                if (size >= entry.getSize().x) {
                    message.text.erase(message.text.begin());
                    Parse(message, tempText);
                }
            }
        }
        resized = false;
    }
}

void Chat::Resize(int width, int height) {
    TileGrid *tileGrid = CC::Get()->GetWindow()->GetTileGrid();

    chatXPos = tileGrid->GetTileSize() * float(Global::FOV);

    float chatWidth = width - chatXPos;
    float entryHeight = height * 0.05f;
    float chatShift = entryHeight * 0.1f;

    if (chatWidth != entry.getSize().x)
        resized = true;

    entry.setSize(sf::Vector2f(chatWidth, entryHeight));
    box.setSize(sf::Vector2f(chatWidth, height * 0.5f - entryHeight));
    textXShift = box.getSize().x * 0.01f;

    chatYPos = height - entry.getSize().y - box.getSize().y - chatShift;
    entry.setPosition(chatXPos, chatYPos + box.getSize().y);
    box.setPosition(chatXPos, chatYPos);

    entryTextXShift = entry.getSize().x * 0.01f, entryTextYShift = entry.getSize().y * 0.1f;
    entryText.setPosition(chatXPos + entryTextXShift, chatYPos + box.getSize().y + entryTextYShift);

    cursor.setPosition(entryText.getGlobalBounds().width + chatXPos + entryTextXShift, chatYPos + box.getSize().y + entryTextYShift);
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
        startPos = counter + 1;
    }
    if (!playerName.empty()) {
        tempText.setStyle(sf::Text::Bold);
        tempText.setString(playerName);
        message.text.push_back(vector<sf::Text>({ tempText }));
        nameWidth = tempText.getGlobalBounds().width;
    }

    tempText.setStyle(sf::Text::Regular);
    size_t string_counter = 0;
    float size = nameWidth + textXShift;
    while (startPos < message.stringText.size()) {
        while (startPos + shiftPos < unsigned(message.stringText.size())) {
            vector<float> letter_sizes = std::move(sizes[message.stringText[startPos + shiftPos]]);
            if (letter_sizes.size()) {
                if (size + letter_sizes[2] >= entry.getSize().x * 0.96f)
                    break;
            }
            else {
                SizeFiller(font, message.stringText[startPos + shiftPos]);
                letter_sizes = std::move(sizes[message.stringText[startPos + shiftPos]]);
                if (size + letter_sizes[2] >= entry.getSize().x * 0.96f)
                    break;
            }
            size += letter_sizes[2];
            shiftPos++;
        }

        size = 0;

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

        if (message.text.size() < ++string_counter)
            message.text.push_back(vector<sf::Text>({ tempText }));
        else
            message.text[string_counter - 1].push_back(tempText);

        startPos += shiftPos;
        if (spaces)
            startPos++;
        shiftPos = 0;
    }
}

//void Chat::draw(sf::RenderTarget &target) const {
//    target.draw(entry);
//    target.draw(box);
//
//    target.draw(entryText);
//
//    float size = 0;
//    //mtx.lock();
//    for (int i = int(boxText.size() - 1); i >= 0; i--) {
//        for (int j = int(boxText[i].text.size() - 1); j >= 0; j--) {
//            size += boxText[i].text[j][0].getGlobalBounds().height + characterSize - 5;
//            if (size > box.getSize().y)
//                break;
//            float boxTextXShift = box.getSize().x * 0.01f;
//            //boxText[i].text[j].setPosition(chatXPos + boxTextXShift, chatYPos + box.getSize().y - size);
//            
//            //target.draw(boxText[i].text[j]);
//        }
//        if (size > box.getSize().y)
//            break;
//    }
//    //mtx.unlock();
//}