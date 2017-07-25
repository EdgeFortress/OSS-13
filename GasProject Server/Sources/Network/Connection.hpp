#pragma once

#include "Shared/Types.hpp"
#include "Shared/ThreadSafeQueue.hpp"

namespace sf {
	class TcpSocket;
}

class Player;
struct ServerCommand;

struct Connection {
	uptr<sf::TcpSocket> socket;
	uf::ThreadSafeQueue<ServerCommand *> commandsToClient;
	sptr<Player> player;
};
