#include <list>
#include <SFML/Network.hpp>

#include "Server.hpp"
#include "network.hpp"
#include "World.hpp"

Game::Game(Server *server) : server(server),
						     world(new World()) {

}

Server::Server() {
	games.push_back(uptr<Game>(new Game(this)));
}

int main() {
	Server server;
	Network s;
	s.main_net->join();

	return 0;
}