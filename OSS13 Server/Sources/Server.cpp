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
#include <Player/Player.h>
#include <Database/UsersDB.hpp>
#include <World/World.hpp>
#include <World/Map.hpp>
#include <World/Objects.hpp>
#include <World/Objects/Control.hpp>

#include <Shared/ErrorHandling.h>

#include "Game.h"

using namespace std;
using namespace sf;

Server::Server() :
	networkController(std::make_unique<NetworkController>()),
	rm(std::make_unique<ResourceManager>()),
	udb(std::make_unique<UsersDB>())
{
	GServer = this;

	plog::ConsoleAppender<plog::MessageOnlyFormatter> appender;
	plog::init(plog::verbose, &appender);

	ASSERT_WITH_MSG(rm->Initialize(), "Failed to Initialize ResourceManager!");
	networkController->Start();
	game = std::make_unique<Game>();
	GGame = game.get();
	while (true) {
		sleep(seconds(1));
	}
}

Player *Server::Authorization(const string &login, const string &password) const {
	if (udb->Check(login, password)) {
		LOGI << "Player is authorized: " << login << " " << password;
		return new Player(login);
	}
	LOGI << "Wrong login data received: " << login << " " << password;
	return nullptr;
}

bool Server::Registration(const string &login, const string &password) const {
	if (udb->Add(login, password)) {
		LOGI << "New player is registrated:" << login << " " << password << endl;
		return true;
	}
	LOGI << "Player is trying make account with wrong account data:" << login << " " << password << endl;
	return false;
}

bool Server::JoinGame(sptr<Player> &player) const {
	return game->AddPlayer(player);
}

ResourceManager *Server::GetRM() const { return rm.get(); }

ResourceManager *IServer::RM() { EXPECT(GServer); return static_cast<Server *>(GServer)->GetRM(); }

int main() {
	Server server;

	return 0;
}

IServer *GServer = nullptr;
