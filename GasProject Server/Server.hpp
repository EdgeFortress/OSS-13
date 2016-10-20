#pragma once

#include <list>
#include <string>

#include <useful.hpp>

class Player;
class Server;
class World;
class UsersDB;

namespace sf {
    class Packet;
}

class Game {
private:
    std::string title;
	Server *server;
	uptr<World> world;

    std::list<Player *> players;

public:
	Game(Server *server, std::string title);

    friend sf::Packet &operator<<(sf::Packet &packet, Game *game);
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
    bool CreateGame(std::string title);
    const std::list<uptr<Game>> * const GetGamesList() const;
    void AddPlayer(Player *player);
};