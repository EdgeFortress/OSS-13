#include <iostream>
#include <list>

#include "Server.hpp"
#include "network.hpp"
#include "World.hpp"
#include "Player.hpp"
#include "users_database.hpp"

Game::Game(Server *server) : server(server),
						     world(new World()) {
    while (true) {
        sleep(seconds(1));
    }
}

Server::Server() : UDB(new UsersDB()) {
    ListeningSocket::Start(this);
	games.push_back(uptr<Game>(new Game(this)));
}

bool Server::Authorization(string &login, string &password) {
    if (UDB->Check(login, password)) {
        cout << "Player is authorized: " << login << ' ' << password << endl;
        return true;
    }
    cout << "Wrong login data received: " << login << ' ' << password << endl;
    return false;
}

bool Server::Registration(string &login, string &password) {
    if (UDB->Add(login, password)) {
        cout << "New player is registrated: " << login << ' ' << password << endl;
        return true;
    }
    return false;
}

void Server::AddPlayer(Player *player) {
    players.push_back(uptr<Player>(player));
}

int main() {
	Server server;

	return 0;
}