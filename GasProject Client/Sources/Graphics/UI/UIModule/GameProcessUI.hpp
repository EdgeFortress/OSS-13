#pragma once

#include <string>
#include <SFML/Graphics.hpp>

#include "Graphics/Chat.hpp"
#include "../Widget/Container.hpp"
#include "UIModule.hpp"

class UI;
class TileGrid;
class ContextMenu;

class InfoLabel;

class GameProcessUI : public UIModule {
public:
    explicit GameProcessUI(UI *ui);
    GameProcessUI(const GameProcessUI &) = delete;
    GameProcessUI &operator=(const GameProcessUI &) = delete;
    virtual ~GameProcessUI() = default;

    void Initialize() override final;

    void Resize(int width, int height) override final;
    void Draw(sf::RenderWindow* renderWindow) override;
    void Update(sf::Time timeElapsed) override final;
    void HandleEvent(sf::Event event) override;

    InfoLabel *GetInfoLabel() const;
    Chat *GetChat() const;
    TileGrid *GetTileGrid() const;

private:
    uptr<InfoLabel> infoLabel;

    Container *functionWindow;
	TileGrid *tileGrid;
	Chat *chat;

    ContextMenu *contextMenu;

    void generateFunctionWindow();
};

class InfoLabel {
private:
    sf::RectangleShape rectangle;
    sf::Text text;
public:
    explicit InfoLabel(const sf::Font &font);
    void Draw(sf::RenderWindow *window) const;
    void CountPosition(int width, int height);
    void SetText(const std::string &s);
};