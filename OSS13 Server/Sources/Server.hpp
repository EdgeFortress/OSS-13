#pragma once

#include <Resources/ResourceManager.hpp>
#include <Network/NetworkController.hpp>
#include <Database/UsersDB.hpp>

#include <IServer.h>

class Game;

class Server : public IServer {
public:
	Server();

// IServer
	Player *Authorization(const std::string &login, const std::string &password) const override;
	bool Registration(const std::string &login, const std::string &password) const override;
	bool JoinGame(sptr<Player> &player) const override;

	ResourceManager *GetRM() const;

private:
	uptr<NetworkController> networkController;
	uptr<ResourceManager> rm;
	uptr<UsersDB> udb;
	uptr<Game> game;
};
