#pragma once

#include <list>
#include <string>
#include <vector>

#include <Resources/ResourceManager.hpp>
#include <Scripts/LuaManager.h>

#include <Shared/Types.hpp>
#include <Shared/Log.hpp>

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

class Chat {
private:
    struct message {
        std::string text;
        bool sended;

        message(const std::string &text) : text(text), sended(false) { }
    };
    std::vector<message> messages;

public:
    Chat() = default;
    Chat(Chat &chat) = delete;
    Chat& operator=(Chat &chat) = delete;
    ~Chat() = default;

    void AddMessage(const std::string &message) {
        messages.push_back(Chat::message(message));
    }

    std::vector<std::string> GetNewMessages() {
        std::vector<std::string> newMessages;

        int i = int(messages.size());
        while (--i >= 0 && !messages[i].sended);

        while (++i < int(messages.size()))
            newMessages.push_back(messages[i].text), messages[i].sended = true;

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

	uptr<LuaManager> lua;

    std::list<sptr<Player>> players;
	std::list<sptr<Player>> disconnectedPlayers;
    std::mutex playersLock;

    Chat chat;

    void gameProcess();

    void update(sf::Time timeElapsed);

public:
    Game(std::string title, int id);

	// True if new player created, false if exist player reconnected
    bool AddPlayer(sptr<Player> &);

	Control *GetStartControl(Player *);

    const uptr<World> &GetWorld() const { return world; }
    const uptr<LuaManager> &GetLM() const { return lua; }

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

    std::list<uptr<Game>> games;

    static Server *instance;

public:
    uptr<UsersDB> UDB;
    uptr<ResourceManager> RM;

    Server();
    Player *Authorization(const std::string &login, const std::string &password);
    bool Registration(const std::string &login, const std::string &password) const;
    bool CreateGame(std::string title);
    const std::list<uptr<Game>> * const GetGamesList() const;
    Game *JoinGame(const int id, sptr<Player> &player) const;

    static Server *Get() { return instance; }

    static uf::Log log;
};