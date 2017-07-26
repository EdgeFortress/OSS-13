#pragma once

#include "Widget/Container.hpp"
#include "UI.hpp"
#include <SFML/Graphics.hpp>

using std::string;

struct GameRow {
    int id;
    string title;
    int num_of_players;

    bool finihedCreation;

    Container *game;

    GameRow(int id, string title, int num_of_players);

    void join();
};

class GameListUI : public UIModule {
private:
    std::list<uptr<Widget>> widgets;
    Container *gameList;

    bool clearList;
    bool newGames;

    float lastGamePos;

    std::list<uptr<GameRow>> games;

    void generateGamelistWindow();

    void update();

public:
    GameListUI(UI *ui);
    GameListUI(const GameListUI &) = delete;
    GameListUI &operator=(const GameListUI &) = delete;
    virtual ~GameListUI() = default;

    void AddGame(int id, string title, int num_of_players);
    void Clear();

    virtual void Resize(int width, int height) final;
    virtual void Draw(sf::RenderWindow* renderWindow) final { for (auto &widget : widgets) widget->Draw(*renderWindow); };
    virtual void Update(sf::Time timeElapsed) final;
    virtual void HandleEvent(sf::Event event) final { for (auto &widget : widgets) widget->HandleEvent(event); }

    virtual void Hide() final;
    virtual void Show() final;
};
