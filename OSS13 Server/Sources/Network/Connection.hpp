#pragma once

#include <Shared/Types.hpp>
#include <Shared/ThreadSafeQueue.hpp>
#include <Shared/Network/Protocol/ServerToClient/Commands.h>

namespace sf {
	class TcpSocket;
}

class Player;
struct ServerCommand;

struct Connection {
	uptr<sf::TcpSocket> socket;
	uf::ThreadSafeQueue<network::protocol::Command *> commandsToClient;
	sptr<Player> player;
};
