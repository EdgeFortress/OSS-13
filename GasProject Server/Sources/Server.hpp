#pragma once

#include <list>
#include <string>
#include <vector>
#include <mutex>

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
public:
	Game();

	// True if new player created, false if exist player reconnected
	bool AddPlayer(sptr<Player> &);

	Control *GetStartControl(Player *);

	const uptr<World> &GetWorld() const { return world; }

	const int GetID() const;
	Chat *GetChat() { return &chat; }

	void SendChatMessages();
	~Game();

	friend sf::Packet &operator<<(sf::Packet &packet, Game &game);

private:
	bool active;
	uptr<std::thread> thread;
	uptr<World> world;

	std::list<sptr<Player>> players;
	std::list<sptr<Player>> disconnectedPlayers;
	std::mutex playersLock;

	Chat chat;

	void gameProcess();

	void update(sf::Time timeElapsed);
};

extern Game *CurThreadGame;

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
