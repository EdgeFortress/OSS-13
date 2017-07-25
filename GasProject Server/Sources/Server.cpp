#include <iostream>
#include <list>
#include <mutex>

#include "SFML/System.hpp"

#include "Server.hpp"
#include "Network/NetworkController.hpp"
#include "Network/Connection.hpp"
#include "Shared/Command.hpp"
#include "World/World.hpp"
#include "World/Objects/Control.hpp"
#include "Player.hpp"
#include "Database/UsersDB.hpp"

using namespace std;
using namespace sf;

Game::Game(string title, int id) : title(title),
                                   id(id),
                                   active(true),
                                   thread(new std::thread(&Game::gameProcess, this)) {

}

void Game::gameProcess() {
    CurThreadGame = this;
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
		for (auto &player : players) {
			if (player->IsConnected()) {
				player->Update();
			} else {

			}
		}

        for (auto iter = players.begin(); iter != players.end();) {
			sptr<Player> player = *iter;
			if (player->IsConnected()) {
				player->Update();
				iter++;
				continue;
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
				continue;
			}
            
        }

        for (wptr<Player> player : players)
            if (sptr<Player> player_s = player.lock())
                player_s->SendGraphicsUpdates();
    }
    SendChatMessages();
}

const int Game::GetID() const { return id; }

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

Server::Server() : UDB(new UsersDB()),
                   networkController(new NetworkController()),
                   new_game_id(1) {
    instance = this;
    networkController->Start();
    CreateGame("One Super Test Game");
    while (true) {
        sleep(seconds(1));
    }
}

Player *Server::Authorization(string &login, string &password) {
    if (UDB->Check(login, password)) {
		Server::log << "Player is authorized:" << login << password << endl;
		return new Player(login);
    }
    Server::log << "Wrong login data received:" << login << password << endl;
	return nullptr;
}

bool Server::Registration(string &login, string &password) const {
    if (UDB->Add(login, password)) {
        Server::log << "New player is registrated:" << login << password << endl;
        return true;
    }
    Server::log << "Player is trying make second accoun with same login:" << login << password << endl;
    return false;
}

bool Server::CreateGame(string title) {
    games.push_back(uptr<Game>(new Game(title, new_game_id)));
    new_game_id++;
    return true;
}

const std::list<uptr<Game>> * const Server::GetGamesList() const {
    return &games;
}

Game *Server::JoinGame(const int id, sptr<Player> &player) const {
    for (auto &game : games) {
		if (game->GetID() == id) {
			game->AddPlayer(player);
			return game.get();
        }
    }
    return nullptr;
}

int main() {
    Server server;

    return 0;
}

uf::Log Server::log;
Server *Server::instance;
thread_local Game *CurThreadGame = nullptr;