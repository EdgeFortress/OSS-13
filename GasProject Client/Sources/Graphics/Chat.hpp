#pragma once

#include <map>
#include <mutex>
#include <string>
#include <vector>

#include <SFML/Graphics.hpp>

#include "UI/Widget/Widget.hpp"

using std::vector;
using std::wstring;

class Chat : public Widget {
private:
    struct Message
    {
        wstring stringText;
        vector<vector<sf::Text>> text;

        Message(const wstring &stringText) : stringText(stringText) { }
    };

    sf::Font font;

    bool active;

    sf::Time cursorTime;
    sf::RectangleShape cursor;

    sf::RectangleShape entry, box;
    float chatXPos, chatYPos;
    int characterSize;

    std::map<sf::Uint32, vector<float>> sizes; //0. Normal size, 1. Bold size, 2. Normal advance size, 3. Bold advance size;
    void SizeFiller(const sf::Font &font, sf::Uint32 c);
    bool resized;
    int scrolled;
    unsigned showPos;
    wstring entryString;
    sf::Text entryText;
    float entryTextXShift, entryTextYShift, textXShift;

    vector<Message> boxText;
    std::mutex mtx;

	virtual void Draw(sf::RenderTarget &target) const final { }
    
    int cursorPos;

public:
    Chat(const sf::Font &font);

    Chat(Chat &chat) = delete;
    Chat& operator=(Chat &chat) = delete;

    ~Chat() = default;

    void Resize(int width, int height);
    void Parse(Message &, sf::Text &);

    void Draw(sf::RenderWindow *window, sf::Time timeElapsed);
    //void Update();

    void SetSymbol(const wchar_t c);
    void DeleteSymbol();

    void MoveLeft();
    void MoveRight();

    void ScrollUp();
    void ScrollDown();

    void Send();
    void AddIncomingMessage(const std::string &message);

    float GetXPos() { return chatXPos; }
    float GetYPos() { return chatYPos; }

    bool GetState() { return active; }
    void SetState(bool active) { this->active = active; }

    //virtual void setPosition(float x, float y) { }
    //virtual void setScale(float factorX, float factorY) { }

    //virtual void setSize(const sf::Vector2f &size) { }

	virtual void Update();
};
