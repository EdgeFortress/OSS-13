#pragma once

#include <vector>
#include <string>

#include <SFML/Graphics.hpp>

using std::vector;
using std::wstring;

class Chat {
private:
    sf::RectangleShape entry, box;
    sf::Text entryText;
    vector<sf::Text> entryTextBuffer;
    vector<sf::Text> boxText;

    float chatXPos, chatYPos;

public:
    Chat(const sf::Font &font);

    Chat(Chat &chat) = delete;
    Chat& operator=(Chat &chat) = delete;

    ~Chat() = default;

    void Resize(int width, int height);

    void Draw(sf::RenderWindow *window);

    void SetSymbol(const wstring &str);
    void Send();
    void AddIncomingMessage(const vector<wstring> &message, const std::string &playerName);
};
