#pragma once

#include <SFML/Graphics.hpp>

#include "UI.hpp"

class GameProcessUI : UIModule {
    sf::RectangleShape infoLabelBackground;
    sf::Text infoLabelText;
public:
    GameProcessUI(UI * ui);
    virtual void Draw() final;
    virtual void Hide() final {};
    virtual void Show() final {};
};