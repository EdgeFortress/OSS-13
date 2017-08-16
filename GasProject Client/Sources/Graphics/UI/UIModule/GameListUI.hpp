#pragma once

#include <string>

#include "../Widget/Container.hpp"
#include <SFML/Graphics.hpp>
#include "UIModule.hpp"

class UI;

struct GameRow;

class GameListUI : public UIModule {
public:
    GameListUI(UI *ui);
    GameListUI(const GameListUI &) = delete;
    GameListUI &operator=(const GameListUI &) = delete;
    virtual ~GameListUI() = default;

    virtual void Initialize() final;

    void AddGame(int id, std::string title, int num_of_players);
    void Clear();

    virtual void Resize(int width, int height) final;
    virtual void Update(sf::Time timeElapsed) final;

private:
    Container *gameList;

	bool waitingNewGames;
    bool clearList;
    bool newGames;

    float lastGamePos;

    std::list<uptr<GameRow>> games;

    void generateGamelistWindow();

    void update();
};

struct GameRow {
    int id;
    std::string title;
    int num_of_players;

    bool finishedCreation;

    Container *game;

    GameRow(int id, std::string title, int num_of_players);

    void join();
};