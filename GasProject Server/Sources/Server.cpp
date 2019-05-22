#include "Server.hpp"

#include <iostream>
#include <list>
#include <mutex>

#include <plog/Log.h>
#include <plog/Appenders/ConsoleAppender.h>
#include <plog/Formatters/MessageOnlyFormatter.h>

#include <SFML/System.hpp>

#include <Network/NetworkController.hpp>
#include <Network/Connection.hpp>
#include <Player.hpp>
#include <Database/UsersDB.hpp>
#include <World/World.hpp>
#include <World/Map.hpp>
#include <World/Objects.hpp>

#include <Shared/Command.hpp>
#include <Shared/ErrorHandling.h>

using namespace std;
using namespace sf;

Game::Game() :
	active(true)
{
	thread = std::make_unique<std::thread>(&Game::gameProcess, this);
}

void Game::gameProcess() {
	world.reset(new World());
	world->FillingWorld();
	Clock clock;
	while (active) {
		update(clock.restart());
		sf::Time timeToSleep = seconds(0.05f) - clock.getElapsedTime(); // 10 ticks per second
		if (timeToSleep > sf::Time::Zero)
			sleep(timeToSleep);
	}
}

void Game::update(sf::Time timeElapsed) {
    world->Update(timeElapsed);
    {
        std::unique_lock<std::mutex> lock(playersLock);
        for (auto iter = players.begin(); iter != players.end();) {
			sptr<Player> player = *iter;
			if (player->IsConnected()) {
				player->Update(timeElapsed);
				iter++;
			} else {
				// If player disconnected, move him into disconnectedPlayers list
				player->Suspend();
				if (iter == players.begin()) {
					disconnectedPlayers.splice(disconnectedPlayers.end(), players, iter);
					iter = players.begin();
				} else {
					auto temp = iter;
					temp--;
					disconnectedPlayers.splice(disconnectedPlayers.end(), players, iter);
					iter = temp++;
				}
			}
        }

        for (wptr<Player> player : players)
            if (sptr<Player> player_s = player.lock())
                player_s->SendGraphicsUpdates(timeElapsed);
    }
    SendChatMessages();
}

bool Game::AddPlayer(sptr<Player> &player) {
    std::unique_lock<std::mutex> lock(playersLock);
	for (auto iter = disconnectedPlayers.begin(); iter != disconnectedPlayers.end(); iter++) {
		sptr<Player> &cur_player = *iter;
		if (cur_player->GetCKey() == player->GetCKey()) {
			sptr<Connection> connection = player->GetConnection();
			connection->player = cur_player;
			cur_player->SetConnection(connection);
			players.splice(players.end(), disconnectedPlayers, iter);

			return false;
		}
	}
    players.push_back(player);
	player->JoinToGame(this);

    return true;
}

Control *Game::GetStartControl(Player *player) {
	return world->CreateNewPlayerCreature()->GetComponent<Control>();
}

void Game::SendChatMessages() {
    std::vector<std::string> messages = chat.GetNewMessages();
    for (auto &player : players)
		for (auto &message : messages)
			player->AddCommandToClient(new SendChatMessageServerCommand(message));
}

Game::~Game() {
    active = false;
    thread->join();
}

Server::Server() :
	networkController(std::make_unique<NetworkController>()),
	RM(std::make_unique<ResourceManager>()),
	UDB(std::make_unique<UsersDB>())
{
	instance = this;

	plog::ConsoleAppender<plog::MessageOnlyFormatter> appender;
	plog::init(plog::verbose, &appender);

	ASSERT_WITH_MSG(RM->Initialize(), "Failed to Initialize ResourceManager!");
	networkController->Start();
	game = std::make_unique<Game>();
	CurThreadGame = game.get();
	while (true) {
		sleep(seconds(1));
	}
}

Player *Server::Authorization(const string &login, const string &password) {
	if (UDB->Check(login, password)) {
		LOGI << "Player is authorized: " << login << " " << password;
		return new Player(login);
	}
	LOGI << "Wrong login data received: " << login << " " << password;
	return nullptr;
}

bool Server::Registration(const string &login, const string &password) const {
	if (UDB->Add(login, password)) {
		LOGI << "New player is registrated:" << login << " " << password << endl;
		return true;
	}
	LOGI << "Player is trying make account with wrong account data:" << login << " " << password << endl;
	return false;
}

bool Server::JoinGame(sptr<Player> &player) const {
	return game->AddPlayer(player);
}

int main() {
	Server server;

	return 0;
}

Server *Server::instance;
Game *CurThreadGame = nullptr;