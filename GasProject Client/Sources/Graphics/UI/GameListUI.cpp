#include "GameListUI.hpp"
#include "Network.hpp"
#include "Widget/Label.hpp"
#include "Widget/Button.hpp"
#include "Client.hpp"
#include "Graphics/Window.hpp"

GameListUI::GameListUI(UI *ui) : UIModule(ui) {
    lastGamePos = 0;
    generateGamelistWindow();
    
    clearList = false;
    newGames = false;
}

void GameListUI::generateGamelistWindow() {
    gameList = new Container(sf::Vector2f(400, 500));
    gameList->SetBackground(sf::Color::Transparent);
    Container *gameBox = new Container(sf::Vector2f(500, 600));
    gameBox->AddItem(new Label(L"Games", ui->GetFont(), 16), sf::Vector2f(10, 10));
    gameBox->AddItem(new Button(L"Update", sf::Vector2f(100, 20), sf::Color::White, ui->GetFont(), std::bind(&GameListUI::update, this)), sf::Vector2f(390, 570));
    gameBox->AddItem(gameList, sf::Vector2f(50, 50));
    gameBox->SetPosition(500, 150);
    gameBox->SetBackground(sf::Color(69, 69, 69));
    widgets.push_back(uptr<Container>(gameBox));
}

void GameListUI::update() {
    Connection::commandQueue.Push(new GameListClientRequest());
}

void GameListUI::AddGame(int id, string title, int num_of_players) {
    games.push_back(uptr<GameRow>(new GameRow(id, title, num_of_players)));
    newGames = true;
}

void GameListUI::Clear() {
    games.clear();
    clearList = true;
}

void GameListUI::Resize(int width, int height) {
    //gamelistWindow->SetRequisition(sf::Vector2f(float(width) / 2,
    //                                            float(height) / 2));
    //gamelistWindow->SetPosition(sf::Vector2f((width - gamelistWindow->GetAllocation().width) / 2,
    //                                         (height - gamelistWindow->GetAllocation().height) / 2));
}

void GameListUI::Update(sf::Time timeElapsed) {
    if (clearList) {
        gameList->Clear();
        clearList = false;
    }
    if (newGames) {
        for (auto &game : games)
            if (!game->finihedCreation) {
                game->game = new Container(sf::Vector2f(400, 30));
                game->game->SetBackground(sf::Color::Transparent);
                game->game->AddItem(new Label(game->title, ui->GetFont(), 16), sf::Vector2f(0, 0));
                game->game->AddItem(new Label(std::to_string(game->num_of_players), ui->GetFont(), 16), sf::Vector2f(300, 0));
                game->game->AddItem(new Button(L"  join", sf::Vector2f(50, 20), sf::Color::White, ui->GetFont(), std::bind(&GameRow::join, game.get())), sf::Vector2f(350, 0));
                gameList->AddItem(game->game, sf::Vector2f(0, lastGamePos));
                game->finihedCreation = true;
            }
        newGames = false;
    }
    for (auto &widget : widgets)
        widget->Update(timeElapsed);
}

void GameListUI::Show() {
    //gamelistWindow->Show(true);
}

void GameListUI::Hide() {
    //gamelistWindow->Show(false);
}

GameRow::GameRow(int id, string title, int num_of_players) :
    id(id), title(title), num_of_players(num_of_players), finihedCreation(false)
{ }

void GameRow::join() {
    Connection::commandQueue.Push(new JoinGameClientCommand(id));
}
