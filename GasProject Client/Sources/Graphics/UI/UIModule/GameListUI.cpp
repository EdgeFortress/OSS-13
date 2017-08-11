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

void GameListUI::generateGamelistWindow() {
    Container *gameBox = new Container(sf::Vector2f(500, 600));
	gameBox->SetPosition(500, 150);
	gameBox->SetBackground(sf::Color(69, 69, 69));
	widgets.push_back(uptr<Container>(gameBox));

	gameList = new Container();
	gameList->SetSize(sf::Vector2f(400, 500));
	gameList->SetBackground(sf::Color::Transparent);
	gameBox->AddItem(gameList, sf::Vector2f(50, 50));

	Label *titleLabel = new Label(L"Games", ui->GetFont(), 16);
    gameBox->AddItem(titleLabel, sf::Vector2f(10, 10));

	Button *updateButton = new Button(L"Update", sf::Vector2f(100, 20), sf::Color::White, ui->GetFont(), 16, std::bind(&GameListUI::update, this));
    gameBox->AddItem(updateButton, sf::Vector2f(390, 570));
}

void GameListUI::update() {
	waitingNewGames = true;
    Connection::commandQueue.Push(new GameListClientRequest());
}

void GameListUI::Initialize() {
	update();
}

void GameListUI::AddGame(int id, string title, int num_of_players) {
    games.push_back(uptr<GameRow>(new GameRow(id, title, num_of_players)));
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
					game->game = new Container(sf::Vector2f(400, 30));
					game->game->SetBackground(sf::Color::Transparent);

					Label *titleLabel = new Label(game->title, ui->GetFont(), 16);
					game->game->AddItem(titleLabel, sf::Vector2f(0, 0));

					Label *numOfPlayersLabel = new Label(std::to_string(game->num_of_players), ui->GetFont(), 16);
					game->game->AddItem(numOfPlayersLabel, sf::Vector2f(200, 0));

					Button *joinButton = new Button(L"join", sf::Vector2f(50, 20), sf::Color::White, ui->GetFont(), 16, std::bind(&GameRow::join, game.get()));
					game->game->AddItem(joinButton, sf::Vector2f(250, 0));

					gameList->AddItem(game->game, sf::Vector2f(0, lastGamePos));
					game->finishedCreation = true;
				}
			newGames = false;
			waitingNewGames = false;
		}
		ui->Unlock();
	}
    
	UIModule::Update(timeElapsed);
}

GameRow::GameRow(int id, string title, int num_of_players) :
    id(id), title(title), num_of_players(num_of_players), finishedCreation(false)
{ }

void GameRow::join() {
    Connection::commandQueue.Push(new JoinGameClientCommand(id));
	CC::Get()->GetWindow()->GetUI()->ChangeModule<GameProcessUI>();
}
