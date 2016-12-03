#include "Graphics/Chat.hpp"
#include "Graphics/Window.hpp"
#include "Network.hpp"

Chat::Chat(const sf::Font &font) {
    entry.setFillColor(sf::Color(31, 31, 31));
    box.setFillColor(sf::Color(60, 60, 60));

    entryText.setFont(font);
    entryText.setCharacterSize(20);
    entryText.setFillColor(sf::Color::Magenta);
    //entryText.setString(L"Приветосы");
    entryText.setString("");
}

void Chat::Draw(sf::RenderWindow *renderWindow) {
    renderWindow->draw(entry);
    renderWindow->draw(box);

    renderWindow->draw(entryText);

    int size = 1;
    for (int i = int(boxText.size() - 1); i >= 0; i--) {
        if (size++ * entryText.getCharacterSize() > box.getSize().y)
            break;
        float boxTextXShift = box.getSize().x * 0.01f, boxTextYShift = entry.getSize().y * 0.1f + entryText.getCharacterSize();
        boxText[i].setPosition(chatXPos + boxTextXShift, chatYPos - entryText.getCharacterSize() * size);
        renderWindow->draw(boxText[i]);
    }
}

void Chat::SetSymbol(const wstring &str) {
    wstring text = entryText.getString() + str;
    //CC::log << text.size() << endl;

    //if (text.size() * entryText.getCharacterSize() > entry.getSize().x) {
    if (text.size() > 70) {
        wstring::iterator cut = text.begin();
        bool spaces = false;
        for (auto i = ++text.begin(); i < text.end(); i++)
            if (*i == ' ')
                cut = i, spaces = true;
        if (spaces) {
            wstring buf;
            buf.insert(buf.end(), text.begin(), cut);
            entryText.setString(buf);
            wstring new_text;
            new_text.insert(new_text.end(), ++cut, text.end());
            text = move(new_text);
        } else
            text = move(str);

        entryTextBuffer.push_back(entryText);
    }

    entryText.setString(text);
}

void Chat::Send() {
    entryTextBuffer.push_back(entryText);

    vector<wstring> message;
    for (auto &text : entryTextBuffer)
        message.push_back(text.getString());
    Connection::commandQueue.Push(new SendChatMessageClientCommand(message));

    entryText.setString(">You:");
    boxText.push_back(entryText);
    boxText.insert(boxText.end(), entryTextBuffer.begin(), entryTextBuffer.end());

    entryTextBuffer.erase(entryTextBuffer.begin(), entryTextBuffer.end());
    entryText.setString("");
    boxText.push_back(entryText);
}

void Chat::AddIncomingMessage(const vector<wstring> &message, const std::string &playerName) {
    vector<sf::Text> localMessage;
    sf::Text text;
    text.setFont(*(entryText.getFont()));
    text.setCharacterSize(20);
    text.setFillColor(sf::Color::Magenta);
    text.setString('>' + playerName + ':');
    localMessage.push_back(text);
    for (auto &str : message)
        text.setString(str), localMessage.push_back(text);
    boxText.insert(boxText.end(), localMessage.begin(), localMessage.end());

    text.setString("");
    boxText.push_back(text);
}

void Chat::Resize(int width, int height) {
    TileGrid *tileGrid = CC::Get()->GetWindow()->GetTileGrid();

    float chatWidth = width * 0.5f, entryHeight = height * 0.05f, chatShift = entryHeight * 0.1f;
    entry.setSize(sf::Vector2f(chatWidth, entryHeight));
    box.setSize(sf::Vector2f(chatWidth, height * 0.5f - entryHeight));

    chatXPos = tileGrid->GetTileSize() * float(Global::FOV), chatYPos = height - entry.getSize().y - chatShift;
    entry.setPosition(chatXPos, chatYPos);
    box.setPosition(chatXPos, chatYPos - box.getSize().y);

    float entryTextXShift = entry.getSize().x * 0.01f, entryTextYShift = entry.getSize().y * 0.1f;
    entryText.setPosition(chatXPos + entryTextXShift, chatYPos + entryTextYShift);
}