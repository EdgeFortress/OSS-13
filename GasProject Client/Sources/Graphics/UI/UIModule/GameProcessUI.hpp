#pragma once

#include <string>
#include <SFML/Graphics.hpp>

#include "Graphics/Chat.hpp"
#include "../Widget/Container.hpp"
#include "UIModule.hpp"

class UI;

class InfoLabel {
private:
    sf::RectangleShape rectangle;
    sf::Text text;
public:
    InfoLabel(const sf::Font &font);
    void Draw(sf::RenderWindow *window);
    void CountPosition(int width, int height);
    void SetText(const std::string &s);
};

class GameProcessUI : public UIModule {
    InfoLabel infoLabel;
    Chat chat;

    Container *functionWindow;
    std::list<uptr<Widget>> widgets;

    void generateFunctionWindow();

public:
    GameProcessUI(UI *ui);

    virtual void Resize(int width, int height) final;
    virtual void Draw(sf::RenderWindow* renderWindow, sf::Time timeElapsed) final;
	virtual void Draw(sf::RenderWindow* renderWindow);
	virtual void HandleEvent(sf::Event event);
    virtual void Update(sf::Time timeElapsed) final;
	InfoLabel *GetInfoLabel();
	Chat *GetChat();
    virtual void Hide() final;
    virtual void Show() final;
};