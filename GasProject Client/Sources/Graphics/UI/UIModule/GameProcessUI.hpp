#pragma once

#include <string>
#include <SFML/Graphics.hpp>

#include "Graphics/Chat.hpp"
#include "../Widget/Container.hpp"
#include "UIModule.hpp"

class UI;
class TileGrid;

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

    Container *functionWindow;
	TileGrid *tileGrid;
	Chat *chat;

    void generateFunctionWindow();

public:
    GameProcessUI(UI *ui);
	GameProcessUI(const GameProcessUI &) = delete;
	GameProcessUI &operator=(const GameProcessUI &) = delete;
	virtual ~GameProcessUI() = default;

	virtual void Initialize() final;

    virtual void Resize(int width, int height) final;
	virtual void Draw(sf::RenderWindow* renderWindow);
	virtual void Update(sf::Time timeElapsed) final;
	virtual void HandleEvent(sf::Event event);

	InfoLabel *GetInfoLabel();
	Chat *GetChat();
	TileGrid *GetTileGrid();
};