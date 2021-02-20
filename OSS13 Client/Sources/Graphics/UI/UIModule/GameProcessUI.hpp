#pragma once

#include <string>
#include <SFML/Graphics.hpp>

#include <Graphics/UI/UIModule/UIModule.hpp>
#include <Graphics/UI/Widget/Container.hpp>
#include <Graphics/UI/Widget/Chat.h>
#include <Graphics/UI/Widget/GameProcess/TileContextMenu.h>

#include <Shared/Network/Protocol/ServerToClient/WorldInfo.h>

class UI;
class TileGrid;

class InfoLabel;

class GameProcessUI : public UIModule {
public:
    explicit GameProcessUI(UI *ui);
    GameProcessUI(const GameProcessUI &) = delete;
    GameProcessUI &operator=(const GameProcessUI &) = delete;
    virtual ~GameProcessUI() = default;

    virtual void Initialize() final;
    void Receive(const std::string &message);

    void Resize(int width, int height) override final;
    void Draw(sf::RenderWindow* renderWindow) override;
    void Update(sf::Time timeElapsed) override final;
    bool HandleEvent(sf::Event event) override;

    void OpenContextMenu();
    void UpdateContextMenu(network::protocol::ContextMenuData &&data);

    void OpenSpawnWindow();
    void UpdateSpawnWindow(std::vector<network::protocol::ObjectType> &&types);

    InfoLabel *GetInfoLabel() const;
    TileGrid *GetTileGrid() const;

private:
    uptr<InfoLabel> infoLabel;

    Container* functionWindow{ nullptr };
    TileGrid* tileGrid{ nullptr };
    TileContextMenu* tileContextMenu{ nullptr };

    Chat* chat;
    std::string inputText;
    bool showInput{};
    bool inputReclaimFocus{};

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