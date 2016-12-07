#pragma once

#include <list>
#include <string>
#include <vector>

#include "Common/Useful.hpp"
#include "World/World.hpp"
#include "PlayerCommand.hpp"

class Player;
class Server;
class NetworkController;
class World;
class Chat;
class UsersDB;

namespace sf {
    class Packet;
    class Time;
}

namespace std {
    class thread;
}

class Chat {
private:
    struct message {
        Player *player;
        std::wstring text;

        message(Player *player, const std::wstring &text) : player(player), text(text) { }
    };
    std::vector<message> messages;

public:
    Chat() = default;
    Chat(Chat &chat) = delete;
    Chat& operator=(Chat &chat) = delete;
    ~Chat() = default;

    void AddMessage(std::wstring &message, Player *player) {
        messages.push_back(Chat::message(player, message));
    }
};

class Game {
private:
    std::string title;
    const int id;
    bool active;
    uptr<std::thread> thread;
    uptr<World> world;

    std::list<wptr<Player>> players;
    std::mutex playersLock;

    Chat chat;

    void gameProcess();

    void update(sf::Time timeElapsed);

public:
    Game(std::string title, int id);

    bool AddPlayer(wptr<Player>);
    //void DeletePlayer(Player *);

    const uptr<World> &GetWorld() { return world; }

    const int GetID() const;
    Chat *GetChat() { return &chat; }

    void SendChatMessage(std::wstring &message, std::string &playerName);
    ~Game();

    friend sf::Packet &operator<<(sf::Packet &packet, Game &game);
};

extern thread_local Game *CurThreadGame;

class Server {
private:
    int new_game_id;

    uptr<NetworkController> networkController;

    std::list<sptr<Player>> players;
    std::mutex playersLock;

    std::list<uptr<Game>> games;

    static Server *instance;

public:
    uptr<UsersDB> UDB;

    Server();
    bool Authorization(std::string &login, std::string &password) const;
    bool Registration(std::string &login, std::string &password) const;
    bool CreateGame(std::string title);
    const std::list<uptr<Game>> * const GetGamesList() const;
    Game *JoinGame(const int id, Player *player) const;
    void AddPlayer(sptr<Player> player);

    static Server *Get() { return instance; }

    static Log log;
};