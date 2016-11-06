#include <iostream>
#include <list>

#include "SFML/System.hpp"

#include "Server.hpp"
#include "Network/Network.hpp"
#include "World/World.hpp"
#include "Player.hpp"
#include "Database/UsersDB.hpp"

using namespace std;
using namespace sf;

Game::Game(Server *server, string title, int id) : title(title),
                                                   id(id),
                                                   server(server),
                                                   active(true),
                                                   thread(new std::thread(gameProcess, this)) {

}

void Game::gameProcess(Game *inst) {
    inst->world.reset(new World());
    while (inst->active) {
        inst->world->Update();
        sleep(seconds(0.1f));
    }
}

Game::~Game() {
    active = false;
    thread->join();
}

const int Game::GetID() const { return id; }

bool Game::AddPlayer(Player *player) {
    players.push_back(player);
    return true;
}

void Game::DeletePlayer(Player *player) { players.remove(player);  }

Server::Server() : UDB(new UsersDB()),
                   new_game_id(1) {
    instance = this;
    ListeningSocket::Start(this);
    CreateGame("One Super Test Game");
    while (true) {
		for (auto i = games.begin(); i != games.end(); i++)
			for (auto j = players.begin(); j != players.end(); j++)
				if (!(j->get()->GetConnection()->IsActive())) {
					i->get()->DeletePlayer(j->get());
					break;
				}
		for (auto i = players.begin(); i != players.end(); i++)
			if (!(i->get()->GetConnection()->IsActive())) {
				players.erase(i);
				break;
			}
        sleep(seconds(1));
    }
}

bool Server::Authorization(string &login, string &password) const {
    if (UDB->Check(login, password)) {
		bool isNew = true;
		for (auto &i : players)
			if (i->GetCKey() == login) {
				isNew = false;
				break;
			}
		if (isNew) {
			Server::log << "Player is authorized:" << login << password << endl;
			return true;
		}
		Server::log << "Player" << login << password << "is trying to authorize second time" <<endl;
		return false;
    }
	Server::log << "Wrong login data received:" << login << password << endl;
    return false;
}

bool Server::Registration(string &login, string &password) const {
    if (UDB->Add(login, password)) {
		Server::log << "New player is registrated:" << login << password << endl;
        return true;
    }
    return false;
}

bool Server::CreateGame(string title) {
    games.push_back(uptr<Game>(new Game(this, title, new_game_id)));
    new_game_id++;
    return true;
}

const std::list<uptr<Game>> * const Server::GetGamesList() const {
    return &games;
}

Game *Server::JoinGame(const int id, Player *player) const {
    Server::log << player->GetCKey() << " connecting game #" << id << endl;
    for (auto &game : games) {
        if (game->GetID() == id) 
            if (game->AddPlayer(player)) return game.get();
            else return nullptr;
    }
    return nullptr;
}

void Server::AddPlayer(Player *player) {
    players.push_back(uptr<Player>(player));
}

Log Server::log;

int main() {
	Server server;

	return 0;
}

Server *Server::instance;