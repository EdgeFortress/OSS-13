#pragma once

#include <vector>
#include <string>

#include <SFML/Graphics.hpp>

using std::vector;
using std::wstring;

class Chat {
private:
    struct Message
    {
        std::string playerName;
        wstring stringText;
        vector<sf::Text> text;

        Message(const std::string &playerName, const wstring &stringText) : playerName(playerName), stringText(stringText) { }
    };

    sf::RectangleShape entry, box;
    float chatXPos, chatYPos;

    unsigned showPos;
    wstring entryString;
    sf::Text entryText;

    vector<Message> boxText;

public:
    Chat(const sf::Font &font);

    Chat(Chat &chat) = delete;
    Chat& operator=(Chat &chat) = delete;

    ~Chat() = default;

    void Resize(int width, int height);
    void Parse(Message &, sf::Text &);

    void Draw(sf::RenderWindow *window);
    void Update();

    void SetSymbol(const wchar_t c);
    void DeleteSymbol();

    void Send();
    void AddIncomingMessage(const wstring &message, const std::string &playerName);
};
