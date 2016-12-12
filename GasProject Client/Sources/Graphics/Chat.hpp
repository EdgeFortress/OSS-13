#pragma once

#include <vector>
#include <string>
#include <mutex>

#include <SFML/Graphics.hpp>

#include "Widget.hpp"

using std::vector;
using std::wstring;

class Chat : public Widget {
private:
    struct Message
    {
        std::string playerName;
        wstring stringText;
        vector<sf::Text> text;

        Message(const std::string &playerName, const wstring &stringText) : playerName(playerName), stringText(stringText) { }
    };

    bool active;

    sf::Time cursorTime;
    sf::RectangleShape cursor;

    sf::RectangleShape entry, box;
    float chatXPos, chatYPos;
    int characterSize;

    unsigned showPos;
    wstring entryString;
    sf::Text entryText;
    float entryTextXShift, entryTextYShift;

    vector<Message> boxText;
    std::mutex mtx;

    virtual void draw(sf::RenderTarget &target, sf::RenderStates states) const final;

public:
    Chat(const sf::Font &font);

    Chat(Chat &chat) = delete;
    Chat& operator=(Chat &chat) = delete;

    ~Chat() = default;

    void Resize(int width, int height);
    void Parse(Message &, sf::Text &);

    void Draw(sf::RenderWindow *window, sf::Time timeElapsed);
    void Update();

    void SetSymbol(const wchar_t c);
    void DeleteSymbol();

    void Send();
    void AddIncomingMessage(const std::string &message, const std::string &playerName);

    float GetXPos() { return chatXPos; }
    float GetYPos() { return chatYPos; }

    bool GetState() { return active; }
    void SetState(bool active) { this->active = active; }

    virtual void setPosition(float x, float y) { }
    virtual void setScale(float factorX, float factorY) { }

    virtual void setSize(const sf::Vector2f &size) { }

    virtual void update() { }
};
