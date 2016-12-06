#pragma once

#include <vector>
#include <string>

#include <SFML/Graphics.hpp>

using std::vector;
using std::wstring;

class Chat {
private:
    sf::RectangleShape entry, box;

    unsigned showPos;
    wstring entryString;
    sf::Text entryText;

    vector<sf::Text> boxText;

    float chatXPos, chatYPos;

public:
    Chat(const sf::Font &font);

    Chat(Chat &chat) = delete;
    Chat& operator=(Chat &chat) = delete;

    ~Chat() = default;

    void Resize(int width, int height);

    void Draw(sf::RenderWindow *window);

    void SetSymbol(const wchar_t c);
    void DeleteSymbol();

    void Send();
    void AddIncomingMessage(wstring &message, const std::string &playerName);
};
