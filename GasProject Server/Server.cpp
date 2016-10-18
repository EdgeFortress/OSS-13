#include <list>

#include "Server.hpp"
#include "network.hpp"
#include "World.hpp"
#include "Player.hpp"
#include "users_database.hpp"

#include <net_const.hpp>

Game::Game(Server *server) : server(server),
						     world(new World()) {
    while (true) {
        sleep(seconds(1));
    }
}

Server::Server() : UDB(new UsersDB()) {
    Network::ListeningSocket::Start(this);
	games.push_back(uptr<Game>(new Game(this)));
}

void Server::AddPlayer(Player *player) {
    players.push_back(uptr<Player>(player));
}

int main() {
	Server server;

	return 0;
}