#pragma once

#include <list>
#include <memory>

#include "net_const.hpp"

//template<class T>
//using uptr = std::unique_ptr<T>;

class Server;
class World;

class Game {
private:
	Server *server;
	uptr<World> world;

public:
	Game(Server *server);
};

class Server {
private:
	std::list<uptr<Game>> games;

public:
	Server();
};