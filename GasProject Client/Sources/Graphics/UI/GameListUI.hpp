#pragma once

#include "UI.hpp"
#include <SFML\Graphics.hpp>

using std::string;

struct GameRow {
    int id;
    string title;
    int num_of_players;

    std::shared_ptr<sfg::Box> box;

    GameRow(int id, string title, int num_of_players);

    void join();
};

class GameListUI : public UIModule {
private:
    sfg::Window::Ptr gamelistWindow;
    sfg::Box::Ptr gamesBox;

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
    virtual void Draw(sf::RenderWindow* renderWindow) final {};
    virtual void Update() final {};

    virtual void Hide() final;
    virtual void Show() final;
};
