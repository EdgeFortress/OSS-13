#pragma once

#include <thread>

#include <SFML/Network/Packet.hpp>

#include <Shared/Types.hpp>

#include <Player.hpp>
#include <Chat.h>

class World;

class Game {
public:
	Game();

	// True if new player created, false if exist player reconnected
	bool AddPlayer(sptr<Player> &);

	Control *GetStartControl(Player *);

	const uptr<World> &GetWorld() const { return world; }

	Chat *GetChat() { return &chat; }

	void SendChatMessages();
	~Game();

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