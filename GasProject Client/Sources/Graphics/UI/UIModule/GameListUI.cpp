#include "GameListUI.hpp"

#include "../UI.hpp"
#include "Network.hpp"
#include "../Widget/Label.hpp"
#include "../Widget/Button.hpp"
#include "Client.hpp"
#include "Graphics/Window.hpp"

#include <SFML/Graphics.hpp>

GameListUI::GameListUI(UI *ui) : UIModule(ui) {
    lastGamePos = 0;
    generateGamelistWindow();

    clearList = false;
    newGames = false;
	waitingNewGames = false;
}

void GameListUI::Initialize() {
    update();
}

void GameListUI::AddGame(int id, string title, int num_of_players) {
    games.push_back(std::make_unique<GameRow>(id, title, num_of_players));
    newGames = true;
}

void GameListUI::Clear() {
    games.clear();
    clearList = true;
    newGames = false;
}

void GameListUI::Resize(int width, int height) {
    //gamelistWindow->SetRequisition(sf::Vector2f(float(width) / 2,
    //                                            float(height) / 2));
    //gamelistWindow->SetPosition(sf::Vector2f((width - gamelistWindow->GetAllocation().width) / 2,
    //                                         (height - gamelistWindow->GetAllocation().height) / 2));
}

void GameListUI::Update(sf::Time timeElapsed) {
    if (waitingNewGames) {
        ui->Lock();
        if (clearList) {
            gameList->Clear();
            clearList = false;
        }
        if (newGames) {
            for (auto &game : games)
                if (!game->finishedCreation) {
                    game->game = new Container({400, 30});
                    game->game->GetStyle().backgroundColor = sf::Color::Transparent;

                    Label *titleLabel = new Label(game->title);
                    titleLabel->GetStyle().fontSize = 16;
                    titleLabel->GetStyle().textColor = sf::Color::White;
                    game->game->AddItem(titleLabel, {0, 0});

                    Label *numOfPlayersLabel = new Label(std::to_string(game->num_of_players));
                    numOfPlayersLabel->GetStyle().fontSize = 16;
                    numOfPlayersLabel->GetStyle().textColor = sf::Color::White;
                    game->game->AddItem(numOfPlayersLabel, {200, 0});

                    Button *joinButton = new Button(L"join", {50, 20}, std::bind(&GameRow::join, game.get()));
                    joinButton->GetStyle().textColor = sf::Color::White;
                    joinButton->GetStyle().fontSize = 16;
                    game->game->AddItem(joinButton, {250, 0});

                    gameList->AddItem(game->game, {0, int(lastGamePos)});
                    game->finishedCreation = true;
                }
            newGames = false;
            waitingNewGames = false;
        }
        ui->Unlock();
    }

    UIModule::Update(timeElapsed);
}

void GameListUI::generateGamelistWindow() {
    Container *gameBox = new Container({500, 600});
	gameBox->SetPosition(500, 150);
	gameBox->GetStyle().backgroundColor = sf::Color(69, 69, 69);
	widgets.push_back(uptr<Container>(gameBox));

	gameList = new Container();
    gameList->SetSize({400, 500});
    gameList->GetStyle().backgroundColor = sf::Color::Transparent;
    gameBox->AddItem(gameList, {50, 50});

	Label *titleLabel = new Label(L"Games");
    titleLabel->GetStyle().fontSize = 16;
    titleLabel->GetStyle().textColor = sf::Color::White;
    gameBox->AddItem(titleLabel, {10, 10});

    Button *updateButton = new Button(L"Update", {100, 20}, std::bind(&GameListUI::update, this));
    updateButton->GetStyle().textColor = sf::Color::White;
    updateButton->GetStyle().fontSize = 16;
    gameBox->AddItem(updateButton, {390, 570});
}

void GameListUI::update() {
	waitingNewGames = true;
    Connection::commandQueue.Push(new GameListClientRequest());
}

GameRow::GameRow(int id, string title, int num_of_players) :
    id(id), title(title), num_of_players(num_of_players), 
    finishedCreation(false), game(nullptr)
{ }

void GameRow::join() const {
    Connection::commandQueue.Push(new JoinGameClientCommand(id));
	CC::Get()->GetWindow()->GetUI()->ChangeModule<GameProcessUI>();
}
