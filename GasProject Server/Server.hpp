#pragma once

#include <list>

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
    void AddPlayer(Player *player);
};