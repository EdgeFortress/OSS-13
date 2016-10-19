#pragma once

#include <list>
#include <string>

#include <useful.hpp>

class Player;
class Server;
class World;
class UsersDB;

class Game {
private:
	Server *server;
	uptr<World> world;

public:
	Game(Server *server);
};

class Server {
private:
    std::list<uptr<Player>> players;
	std::list<uptr<Game>> games;

public:
    uptr<UsersDB> UDB;

	Server();
    bool Authorization(std::string &login, std::string &password);
    bool Registration(std::string &login, std::string &password);
    void AddPlayer(Player *player);
};