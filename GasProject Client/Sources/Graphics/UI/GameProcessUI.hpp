#pragma once

#include <SFML/Graphics.hpp>

#include "UI.hpp"
#include "Graphics/Chat.hpp"
#include "Widget/Container.hpp"

class InfoLabel {
private:
    sf::RectangleShape rectangle;
    sf::Text text;
public:
    InfoLabel(const sf::Font &font);
    void Draw(sf::RenderWindow *window);
    void CountPosition(int width, int height);
    void SetText(const string &s);
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
    virtual void Draw(sf::RenderWindow* renderWindow) { for (auto &widget : widgets) widget->Draw(*renderWindow); }
    virtual void HandleEvent(sf::Event event) { for (auto &widget : widgets) widget->HandleEvent(event); }
    virtual void Update(sf::Time timeElapsed) final;
    InfoLabel *GetInfoLabel() { return &infoLabel; }
    Chat *GetChat() { return &chat; }
    virtual void Hide() final;
    virtual void Show() final;
};