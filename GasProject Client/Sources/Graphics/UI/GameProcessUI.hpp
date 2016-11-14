#pragma once

#include <SFML/Graphics.hpp>

#include "UI.hpp"

class GameProcessUI : public UIModule {
    sf::RectangleShape infoLabelBackground;
    sf::Text infoLabelText;
    void drawInfoPanel(sf::RenderWindow* renderWindow);
public:
    GameProcessUI(UI * ui);

    virtual void Resize(int width, int height) final;
    virtual void Draw(sf::RenderWindow* renderWindow) final;

    virtual void Hide() final {};
    virtual void Show() final {};
};