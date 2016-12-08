#pragma once

#include <list>
#include <string>
#include <vector>

#include "Common/Useful.hpp"

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
        std::string playerName;
        std::wstring text;
        bool sended;

        message(const std::string &playerName, const std::wstring &text) : playerName(playerName), text(text), sended(false) { }
    };
    std::vector<message> messages;

public:
    Chat() = default;
    Chat(Chat &chat) = delete;
    Chat& operator=(Chat &chat) = delete;
    ~Chat() = default;

    void AddMessage(std::wstring &message, std::string &playerName) {
        messages.push_back(Chat::message(playerName, message));
    }

    std::vector<std::pair<std::string, std::wstring>> GetNewMessages() {
        std::vector<std::pair<std::string, std::wstring>> newMessages;

        int i = int(messages.size());
        while (--i >= 0 && !messages[i].sended);

        while (++i < int(messages.size()))
            newMessages.push_back(std::make_pair(messages[i].playerName, messages[i].text)), messages[i].sended = true;

        return std::move(newMessages);
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

    const uptr<World> &GetWorld() { return world; }

    const int GetID() const;
    Chat *GetChat() { return &chat; }

    void SendChatMessages();
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