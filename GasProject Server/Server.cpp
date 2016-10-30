#include <iostream>
#include <list>

#include "SFML/System.hpp"

#include "Server.hpp"
#include "network.hpp"
#include "World.hpp"
#include "Player.hpp"
#include "users_database.hpp"

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

Server::Server() : UDB(new UsersDB()),
                   new_game_id(1) {
    ListeningSocket::Start(this);
    while (true) {
        sleep(seconds(1));
    }
}

bool Server::Authorization(string &login, string &password) const {
    if (UDB->Check(login, password)) {
		Log::log << "Player is authorized:" << login << password << endl;
        return true;
    }
	Log::log << "Wrong login data received:" << login << password << endl;
    return false;
}

bool Server::Registration(string &login, string &password) const {
    if (UDB->Add(login, password)) {
		Log::log << "New player is registrated:" << login << password << endl;
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

Game *Server::JoinGame(const int id, Player *player) const{
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

Log Log::log;

int main() {
	Server server;

	return 0;
}