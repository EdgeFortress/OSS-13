#pragma once

#include <list>
#include <string>
#include <vector>

#include "Common/Useful.hpp"
#include "World/World.hpp"
#include "PlayerCommand.hpp"

class Player;
class Server;
class World;
class UsersDB;

namespace sf {
    class Packet;
    class Time;
}

namespace std {
    class thread;
}

class Game {
private:
    std::string title;
    const int id;
    Server *server;
    bool active;
    uptr<std::thread> thread;
    uptr<World> world;

    std::list<Player *> players;
    std::mutex playersLock;

    static void gameProcess(Game *);

    void update(sf::Time timeElapsed);

public:
    Game(Server *server, std::string title, int id);

    bool AddPlayer(Player *);
    void DeletePlayer(Player *);

    const uptr<World> &GetWorld() { return world; }

    const int GetID() const;

    ~Game();

    friend sf::Packet &operator<<(sf::Packet &packet, Game &game);
};

extern thread_local Game *CurThreadGame;

class Server {
private:
    int new_game_id;
    std::list<uptr<Player>> players;
    std::list<uptr<Game>> games;

    static Server *instance;

public:
    uptr<UsersDB> UDB;

    Server();
    bool Authorization(std::string &login, std::string &password) const;
    bool Registration(std::string &login, std::string &password) const;
    bool CreateGame(std::string title);
    const std::list<uptr<Game>> * const GetGamesList() const;
    void JoinGame(const int id, Player *player) const;
    void AddPlayer(Player *player);

    static Server *Get() { return instance; }

    static Log log;
};