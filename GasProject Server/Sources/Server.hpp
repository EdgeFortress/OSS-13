#pragma once

#include <list>
#include <string>
#include <vector>
#include <mutex>

#include <Game.h>
#include <Resources/ResourceManager.hpp>

#include <Shared/Types.hpp>

class Player;
class Server;
class NetworkController;
class World;
class Chat;
class UsersDB;
class Control;

namespace sf {
    class Packet;
    class Time;
}

namespace std {
    class thread;
}

class Server {
public:
	uptr<UsersDB> UDB;
	uptr<ResourceManager> RM;

	Server();
	Player *Authorization(const std::string &login, const std::string &password);
	bool Registration(const std::string &login, const std::string &password) const;
	bool JoinGame(sptr<Player> &player) const;

	static Server *Get() { return instance; }
	Game *GetGame() { return game.get(); }

private:
	uptr<NetworkController> networkController;
	uptr<Game> game;
	static Server *instance;
};
